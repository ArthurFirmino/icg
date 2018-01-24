#include "OpenGP/Image/Image.h"
#include "bmpwrite.h"

using namespace OpenGP;
using Colour = Vec3;

struct Triangle {
    Vec3 v1, v2, v3;
};

struct Line {
    Vec3 v1, v2;
};

float triangleArea(Vec3 v1, Vec3 v2, Vec3 v3) {
    return 0; /// TODO: Calculate the area of the triangle
}

void rasterize(Triangle t, Image<Colour> &image) {

    // Vectors projected onto z=0 plane
    Vec3 s1 = Vec3(t.v1(0), t.v1(1), 0);
    Vec3 s2 = Vec3(t.v2(0), t.v2(1), 0);
    Vec3 s3 = Vec3(t.v3(0), t.v3(1), 0);

    /// OPTIONAL: Restrict these bounds to only rasterize triangle bounding box
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
            float alpha = 0.0f;
            float beta = 0.0f;
            float gamma = 0.0f;

            if (false /** TODO: check if fragment is inside triangle **/) {
                image(j,i) = Colour(0.5f,1.0f,0);
            }
        }
    }

}

void rasterize(Line l, Image<Colour> &image) {

    // Vectors projected onto z=0 plane
    Vec3 s1 = Vec3(l.v1(0), l.v1(1), 0);
    Vec3 s2 = Vec3(l.v2(0), l.v2(1), 0);

    /// TODO: Calculate error delta
    float deltax = 0.0f;
    float deltay = 0.0f;
    float deltaerr = 0.0f;
    float error = 0.0f;

    /// TODO: Calculate starting pixel y-coordinate
    int y = 0;

    /// TODO: Calculate for-loop bounds (pixel x-coorinate bounds)
    for ( int x = 0 ; x <= -1 ; ++x) {
        image(y,x) = Colour(0.5f,1.0f,0);
        /// TODO: Rest of Bresenham's line algorithm

    }
}

int main(int, char**){

    int wResolution = 500;
    int hResolution = 500;

    Image<Colour> image(wResolution, hResolution);

    Triangle t = {
        Vec3(-0.3f,0.0f,0.0f), Vec3(0.0f,0.6f,0.0f), Vec3(0.3f,0.0f,0.0f)
    };

    Line l = {
        Vec3(-0.5f, -0.75f, 0.0f), Vec3(0.5f, -0.25f, 0.0f)
    };

    rasterize(t, image);
    rasterize(l, image);

    bmpwrite("../../out.bmp", image);
    imshow(image);

    return EXIT_SUCCESS;
}
