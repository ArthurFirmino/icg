#include <math.h>
#include "OpenGP/Image/Image.h"
#include <OpenGP/GL/Eigen.h>
#include "bmpwrite.h"


using namespace OpenGP;
using Colour = Vec3;

struct Triangle {
    Vec3 v1, v2, v3, n1, n2, n3;
};

float triangleArea(Vec3 v1, Vec3 v2, Vec3 v3) {

    return 0; /// TODO: Calculate the area of the triangle

}

Colour shadeFragment(Vec3 normal) {
    float l = std::min(std::max(normal.dot(Vec3(0,1,0)) + 0.3f, 0.0f), 1.0f);
    Vec3 c = Vec3(127,255,0) * l / 255.0f;
    return c.cast<float>();
}

void rasterize(Triangle t, Image<Colour> &image, Image<float> &depthBuffer) {

    // Vectors projected onto z=0 plane
    Vec3 s1 = Vec3(t.v1(0), t.v1(1), 0);
    Vec3 s2 = Vec3(t.v2(0), t.v2(1), 0);
    Vec3 s3 = Vec3(t.v3(0), t.v3(1), 0);

    /// TODO: Restrict these bounds to only rasterize triangle bounding box (Optional Optimization)
    int iMin = 0;
    int iMax = image.cols();
    int jMin = 0;
    int jMax = image.rows();

    float totalArea = triangleArea(s1, s2, s3);

    for (int i = iMin;i < iMax;i++) {
        for (int j = jMin;j < jMax;j++) {

            // Location of fragment in image space
            Vec3 pt = Vec3(((float)i) / image.cols(), ((float)j) / image.rows(), 0.0);
            pt = 2 * (pt - Vec3(0.5,0.5,0));

            /// TODO: Calculate barycentric coordinates of the fragment within current triangle
            float alpha = 0;
            float beta = 0;
            float gamma = 0;

            /// TODO: Calculate depth value of fragment (Use z-component of vertex positions)
            float depth = 0;

            if (false /** TODO: check if fragment is inside triangle
                                and not behind previous fragment **/) {

                /// TODO: Calculate normal vector of fragment (very similar to depth calculation)
                Vec3 normal = Vec3(0, 0, 0);
                Colour c = shadeFragment(normal);

                depthBuffer(j,i) = depth;
                image(j,i) = c;
            }
        }
    }

}

int main(int, char**){

    std::string meshName = "tet.obj";
    //std::string meshName = "bunny.obj";

    SurfaceMesh mesh;
    if (!mesh.read(meshName)) {
        std::cerr << "Failed to read input mesh" << std::endl;
        return EXIT_FAILURE;
    }

    mesh.update_vertex_normals();

    auto vpoint = mesh.vertex_property<Vec3>("v:point");
    auto vnormal = mesh.vertex_property<Vec3>("v:normal");

    int wResolution = 500;
    int hResolution = 500;

    Image<Colour> image(wResolution, hResolution);
    Image<float> depthBuffer(wResolution, hResolution);

    // Set initial depth to large value
    for (int i = 0;i < image.cols();i++) {
        for (int j = 0;j < image.rows();j++) {
            depthBuffer(i,j) = std::numeric_limits<float>::max();
        }
    }

    float fieldOfView = 80; // degrees
    float near = 0.01;
    float far = 10;

    Mat4x4 projection = OpenGP::perspective(fieldOfView, (float)wResolution/(float)hResolution, 0.01, 5);

    Vec3 eye = Vec3(0,0.3,3);
    Vec3 focus = Vec3(0,0,0);
    Vec3 up = Vec3(0,1,0);

    Mat4x4 view = OpenGP::lookAt(eye, focus, up);

    Mat4x4 model = Mat4x4::Identity();

    Mat4x4 mvp = projection * view * model;

    for (SurfaceMesh::Face tri : mesh.faces()) {
        auto h1 = mesh.halfedge(tri);
        auto h2 = mesh.next_halfedge(h1);
        auto h3 = mesh.next_halfedge(h2);

        SurfaceMesh::Halfedge halfedges[3] = {
            h1, h2, h3
        };

        Vec3 vertices[3] = {
            Vec3(0,0,0), Vec3(0,0,0), Vec3(0,0,0)
        };

        Vec3 normals[3] = {
            Vec3(0,0,0), Vec3(0,0,0), Vec3(0,0,0)
        };

        for (int i = 0;i < 3;i++) {
            auto vi = mesh.to_vertex(halfedges[i]);
            Vec3 v = vpoint[vi];
            Vec4 ssv = mvp * Vec4(v(0), v(1), v(2), 1);
            vertices[i] = Vec3(ssv(0) / ssv(3), ssv(1) / ssv(3), ssv(2) / ssv(3));
            normals[i] = vnormal[vi];
        }

        Triangle t = {
            vertices[0], vertices[1], vertices[2],
            normals[0], normals[1], normals[2]
        };

        rasterize(t, image, depthBuffer);
    }

    bmpwrite("../../out.bmp", image);
    imshow(image);

    return EXIT_SUCCESS;
}
