/* Contributors: Shannon Liu & Kevin Bui*/

/*** Public: For header file ***/

/* On macOS, compile with...
    clang 180mainEffect.c 040pixel.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit
On Ubuntu, compile with...
    cc 160mainAbstracted.c 040pixel.o -lglfw -lGL -lm -ldl
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLFW/glfw3.h>

#include "040pixel.h"

#include "080vector.c"
#include "100matrix.c"
#include "150texture.c"
#include "170shading.c"

/* It is important that shadeFragment correctly parse the data that we give it. 
To help avoid errors in communication, we set up some constants. Notice that the 
documentation for the new triRender requires ATTRX to be 0 and ATTRY to be 1. */
#define ATTRX 0
#define ATTRY 1
#define ATTRS 2
#define ATTRT 3
#define ATTRR 4
#define ATTRG 5
#define ATTRB 6
#define UNIFR 0
#define UNIFG 1
#define UNIFB 2
#define TEXR 0
#define TEXG 1
#define TEXB 2

/* attr has already been interpolated from the vertex attributes. tex is an 
array of texNum elements, each of which is of type texTexture *. rgb is the 
output parameter. The other parameters should be self-explanatory. This function 
is not allowed to read from any variables other than its arguments and local 
variables. It is not allowed to write to any variables except for its rgb 
parameter and local variables. In particular, it cannot use any global 
variables. */
void shadeFragment(
        int unifDim, const double unif[], int texNum, const texTexture *tex[], 
        int attrDim, const double attr[], double rgb[3]) {
		
        //create arrays for interpolated RGB, texture1's RGB, texture 2's RGB, and overlaid texture
		double interpolatedRGB[3] = {attr[4], attr[5], attr[6]}, texture1[3], 
        texture2[3], overlaidTexture[3];
		
        //Sample the colors of texture1
        texSample(tex[0],attr[2], attr[3],texture1);
        //texSample(tex[1],attr[2], attr[3], texture2);

        //Sample the texture of texture2
        texSample(tex[1], texture1[0], texture1[1], overlaidTexture);
        
		//set rgb to uniform colors for modulating
		rgb[0] = unif[0];
		rgb[1] = unif[1];
		rgb[2] = unif[2];
		vecModulate(3, rgb, overlaidTexture, rgb);    
		vecModulate(3, rgb, interpolatedRGB, rgb);
}

/* We have to include triangle.c after defining shadeFragment, because triRender 
refers to shadeFragment. (Later in the course we handle this issue better.) */
#include "170triangle.c"

/* This struct is initialized in main() below. */
shaShading sha;
/* Here we make an array of one texTexture pointer, in such a way that the 
const qualifier can be enforced throughout the surrounding code. C is confusing 
for stuff like this. Don't worry about mastering C at this level. It doesn't 
come up much in our course. */
texTexture texture1, texture2;
const texTexture *textures[2] = {&texture1, &texture2};
const texTexture **tex = textures;

void render(void) {
	pixClearRGB(0.0, 0.0, 0.0);
	double a[7] = {400.0, 100.0, 1.0, 1.0, 1.0, 1.0, 1.0};
	double b[7] = {500.0, 500.0, 0.0, 1.0, 1.0, 1.0, 1.0};
	double c[7] = {30.0, 30.0, 0.0, 0.0, 1.0, 1.0, 1.0};
	double unif[3] = {1.0, 1.0, 1.0};
	triRender(&sha, unif, tex, a, b, c);
}

void handleKeyUp(int key, int shiftIsDown, int controlIsDown, 
		int altOptionIsDown, int superCommandIsDown) {
	if (key == GLFW_KEY_ENTER) {
		if (texture1.filtering == texLINEAR)
			texSetFiltering(&texture1, texNEAREST);
		else
			texSetFiltering(&texture1, texLINEAR);
		render();
	}
}

void handleTimeStep(double oldTime, double newTime) {
	if (floor(newTime) - floor(oldTime) >= 1.0)
		printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
}

int main(void) {
	if (pixInitialize(512, 512, "Abstracted") != 0)
		return 1;
    
    //initialize grumpy cat as texture1
	if (texInitializeFile(&texture1, "./140imageCat.jpg") != 0) {
		pixFinalize();
		return 2;
	}

    //initialize nyan cat as texture2
    if (texInitializeFile(&texture2, "./nyan.jpg") != 0) {
        pixFinalize();
        return 2;
    }

    texSetFiltering(&texture1, texNEAREST);
    texSetLeftRight(&texture1, texREPEAT);
    texSetTopBottom(&texture1, texREPEAT);

    texSetFiltering(&texture2, texNEAREST);
    texSetLeftRight(&texture2, texREPEAT);
    texSetTopBottom(&texture2, texREPEAT);
    sha.unifDim = 3;
    sha.attrDim = 2 + 2 + 3;
    sha.texNum = 2;
    render();
    pixSetKeyUpHandler(handleKeyUp);
    pixSetTimeStepHandler(handleTimeStep);
    pixRun();
    texFinalize(&texture1);
    texFinalize(&texture2);
    pixFinalize();
    return 0;
}