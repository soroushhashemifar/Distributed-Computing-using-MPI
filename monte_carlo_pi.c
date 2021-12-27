/**********************************************************************************************
* Estimating PI number (Serialized)
*
* Soroush Hashemifar - Iran University of Science and Technology (IUST), Tehran, Iran.
* 
*
* To run the code, just run this:
* gcc -o test monte_carlo_pi.c  -lm
* time ./test
* 
************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
void main(int argc, char* argv[])
{
    double niter = 100000000;
    double x, y;
    int i;
    int count=0;
    double z;
    double pi;
    srand(4);
    
    //main loop
    for (i=0; i<niter; ++i)
    {
        //get random points
        x = (double)random()/RAND_MAX;
        y = (double)random()/RAND_MAX;
        z = sqrt((x*x)+(y*y));
        //check to see if point is in unit circle
        if (z<=1)
        {
            ++count;
        }
    }
    pi = ((double)count/(double)niter)*4.0;          //p = 4(m/n)
    printf("Pi: %f\n", pi);
}