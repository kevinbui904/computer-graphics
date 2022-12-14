/* Contributors: Shannon Liu & Kevin Bui*/

#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>

void linearInterpolation(const double a[2], const double b[2], const double c[2], const double x[2], 
        const double rgb[3], const double alpha[2], const double AInv[2][2],const double betaMinAlpha[2], const double gamMinAlpha[2], double chi[2]){
    
    //create empty vectors and matrix to input
    double xMinusA[2], pq[2], pMult[2], qMult[2], pqMultAdd[2];
    
    // subtract vector x from vector a
    vecSubtract(2, x, a, xMinusA);

    //multiply inverse of matrix A by x-a
    mat221Multiply(AInv, xMinusA, pq);

    //p*(beta - alpha)
    vecScale(2, pq[0], betaMinAlpha, pMult);

    //q*(gamma - alpha)
    vecScale(2, pq[1], gamMinAlpha, qMult);

    //p*(beta - alpha) + q*(gamma - alpha)
    vecAdd(2, pMult, qMult, pqMultAdd);

    //alpha + (p*(beta - alpha) + q*(gamma - alpha))
    vecAdd(2, alpha, pqMultAdd, chi);

    //modulate pixel color to find final color
    //vecModulate(3, chi, rgb, final);

}

void triRenderALeft(const double a[2], const double b[2], const double c[2], 
        const double rgb[3], const texTexture *tex, const double alpha[2], const double beta[2], 
        const double gam[2]){

    // Calculate matrix A based on vectors a, b, c
    double A[2][2] = {{b[0]-a[0], c[0]-a[0]}, {b[1]-a[1], c[1]-a[1]}};
    //invert matrix A
    double AInv[2][2];
    mat22Invert(A, AInv);

    double betaMinAlpha[2], gamMinAlpha[2];

    //beta - alpha
    vecSubtract(2, beta, alpha, betaMinAlpha);

    //gamma - alpha
    vecSubtract(2, gam, alpha, gamMinAlpha);

    double x[2], sample[3], chi[2], final[3];

    //Case if a0 = c0, prevent division by 0
    if (a[0] == c[0]){
        for(int x0 = ceil(a[0]); x0<=floor(b[0]); x0++){

        double upper = floor(c[1] + (b[1]-c[1])/(b[0]-c[0])*(x0-c[0]));
        double lower = ceil(a[1]+(b[1]-a[1])/(b[0]-a[0])*(x0-a[0]));

        for(int x1 = lower; x1 <= upper; x1++){
            x[0]=x0;
            x[1]=x1;

            linearInterpolation(a, b, c, x, rgb, alpha, AInv, betaMinAlpha, gamMinAlpha, chi);

            texSample(tex, chi[0], chi[1], sample);
            vecModulate(3, sample, rgb, final);
            pixSetRGB(x0, x1, final[0], final[1], final[2]);
        }
        }
    }
    //Case if a0 = b0, prevent division by 0
    else if(a[0]==b[0]){
        for(int x0 = ceil(a[0]); x0<=floor(c[0]); x0++){

            double upper = floor(a[1]+(c[1]-a[1])/(c[0]-a[0])*(x0-a[0]));
            double lower = ceil(b[1] + (c[1]-b[1])/(c[0]-b[0])*(x0-b[0]));

            for(int x1 = lower; x1 <= upper; x1++){
                x[0]=x0;
                x[1]=x1;

                linearInterpolation(a, b, c, x, rgb, alpha, AInv, betaMinAlpha, gamMinAlpha, chi);

                texSample(tex, chi[0], chi[1], sample);
                vecModulate(3, sample, rgb, final);
                pixSetRGB(x0, x1, final[0], final[1], final[2]);
            }
        }
    }
    //Case if c0 <= b0
    else if(c[0] <= b[0]){
        for(int x0 = ceil(a[0]); x0<=floor(c[0]); x0++){

            double upper = floor(a[1]+((c[1]-a[1])/(c[0]-a[0])*(x0-a[0])));
            double lower = ceil(a[1] + ((b[1]-a[1])/(b[0]-a[0])*(x0-a[0])));

            for(int x1 = lower; x1 <= upper; x1++){
                x[0]=x0;
                x[1]=x1;

                linearInterpolation(a, b, c, x, rgb, alpha, AInv, betaMinAlpha, gamMinAlpha, chi);

                texSample(tex, chi[0], chi[1], sample);
                vecModulate(3, sample, rgb, final);
                pixSetRGB(x0, x1, final[0], final[1], final[2]);
            }
        }

        for(int x0 = floor(c[0])+1; x0<=floor(b[0]); x0++){

            double upper = floor(c[1] + (b[1]-c[1])/(b[0]-c[0])*(x0-c[0]));
            double lower = ceil(a[1]+(b[1]-a[1])/(b[0]-a[0])*(x0-a[0]));

            for(int x1 = lower; x1 <= upper; x1++){
                x[0]=x0;
                x[1]=x1;

                linearInterpolation(a, b, c, x, rgb, alpha, AInv, betaMinAlpha, gamMinAlpha, chi);

                texSample(tex, chi[0], chi[1], sample);
                vecModulate(3, sample, rgb, final);
                pixSetRGB(x0, x1, final[0], final[1], final[2]);
            }
        }
    }
    //case if b0 < c0
    else{
        for(int x0 = ceil(a[0]); x0<=floor(b[0]); x0++){

            double upper = floor(a[1]+((c[1]-a[1])/(c[0]-a[0])*(x0-a[0])));
            double lower = ceil(a[1] + ((b[1]-a[1])/(b[0]-a[0])*(x0-a[0])));

            for(int x1 = lower; x1 <= upper; x1++){
                x[0]=x0;
                x[1]=x1;

                linearInterpolation(a, b, c, x, rgb, alpha, AInv, betaMinAlpha, gamMinAlpha, chi);

                texSample(tex, chi[0], chi[1], sample);
                vecModulate(3, sample, rgb, final);
                pixSetRGB(x0, x1, final[0], final[1], final[2]);
            }
        }

        for(int x0 = floor(b[0])+1; x0<=floor(c[0]); x0++){

            double upper = floor(a[1]+(c[1]-a[1])/(c[0]-a[0])*(x0-a[0]));
            double lower = ceil(b[1] + (c[1]-b[1])/(c[0]-b[0])*(x0-b[0]));

            for(int x1 = lower; x1 <= upper; x1++){
                x[0]=x0;
                x[1]=x1;

                linearInterpolation(a, b, c, x, rgb, alpha, AInv, betaMinAlpha, gamMinAlpha, chi);

                texSample(tex, chi[0], chi[1], sample);
                vecModulate(3, sample, rgb, final);
                pixSetRGB(x0, x1, final[0], final[1], final[2]);
            }
        }
    }

}

void triRender(
        const double a[2], const double b[2], const double c[2], 
        const double rgb[3], const texTexture *tex, const double alpha[2], 
        const double beta[2], const double gam[2]){

    //reorder points to make a0 the leftmost
    if(a[0] <= b[0] && a[0] <= c[0]){
        triRenderALeft(a, b, c, rgb, tex, alpha, beta, gam);
    }
    else if(b[0] <= c[0] && b[0] <= a[0]){
        triRenderALeft(b, c, a, rgb, tex, beta, gam, alpha);
    }
    else{
        triRenderALeft(c, a, b, rgb, tex, gam, alpha, beta);
    }
	
}