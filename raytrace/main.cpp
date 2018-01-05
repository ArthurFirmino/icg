#include "OpenGP/Image/Image.h"
#include "bmpwrite.h"

using namespace OpenGP;

using Colour = Vec3; // RGB Value
Colour red() { return Colour(1.0f, 0.0f, 0.0f); }
Colour white() { return Colour(1.0f, 1.0f, 1.0f); }
Colour black() { return Colour(0.0f, 0.0f, 0.0f); }

int main(int, char**){

    int wResolution = 640;
    int hResolution = 480;
    // #rows = hResolution, #cols = wResolution
    Image<Colour> image(hResolution, wResolution);

    /// TODO: define camera position and sphere position here
    // example: Vec3 spherePos = Vec3(0, 5, 0);

    for (int row = 0; row < image.rows(); ++row) {
        for (int col = 0; col < image.cols(); ++col) {

            /// TODO: build primary rays
            // example: Vec3 o = Vec3(0,0,0);
            
            /// TODO: ray sphere intersection and shading

            /// EXAMPLE: using "image(row,col) = colour;" to set pixel values
            if (row > 0 && row < 200 && col > 0 && col < 200)  {
                image(row,col) = red();
            } else {
                image(row,col) = white();
            }
       }
    }

    bmpwrite("../../out.bmp", image);
    imshow(image);

    return EXIT_SUCCESS;
}
