/* Contributors: Shannon Liu & Kevin Bui*/

/* On macOS, compile with...
    clang 140mainTexturing.c 040pixel.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit
On Ubuntu, compile with...
    cc 100mainTesting.c 040pixel.o -lglfw -lGL -lm -ldl
*/

#include <stdio.h>
#include <math.h>
#include "040pixel.h"

#include "080vector.c"
#include "100matrix.c"
#include "140texture.c"
#include "140triangle.c"

double a[2] = {60.0, 100.0};
double b[2] = {244.0, 50.0};
double c[2] = {380.0, 480.0};
double rgb[3] = {1.0, 1.0, 1.0};
double alpha[2] = {0.0, 1.0};
double beta[2] = {1.0, 0.0};
double gam[2] = {1.0, 1.0};
double angle = 0.0;
texTexture img;

void handleTimeStep(double oldTime, double newTime) {

    if (floor(newTime) - floor(oldTime) >= 1.0)
        printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));

        //updates rendering when every time update
        triRender(a, b, c, rgb, &img, alpha, beta, gam);

        //texSetFiltering(&img, filtering);
        // double transl[2] = {256.0, 256.0};
        // double aa[2], bb[2], cc[2], rrggbb[3], rot[2][2];
        // angle += (newTime - oldTime) / 10.0;
        // mat22Rotation(angle, rot);
        // mat221Multiply(rot, a, aa);
        // mat221Multiply(rot, b, bb);
        // mat221Multiply(rot, c, cc);
        // vecAdd(2, transl, aa, aa);
        // vecAdd(2, transl, bb, bb);
        // vecAdd(2, transl, cc, cc);
        // vecScale(3, (2.0 + sin(newTime)) / 3.0, rgb, rrggbb);
        // pixClearRGB(0.0, 0.0, 0.0);
        // triRender(aa, bb, cc, rrggbb, &img, alpha, beta, gam);
}

int main(void) {
    if (pixInitialize(512, 512, "Testing") != 0)
        return 1;

    //texInitializeFile(&img, "./nyan.png");
    texInitializeFile(&img, "./140imageCat.jpg");
    pixSetTimeStepHandler(handleTimeStep);

    pixClearRGB(0.0, 0.0, 0.0);
    pixRun();
    
    texFinalize(&img);
    pixFinalize();
    return 0;
}