/**********************************************************************************************
* Estimating PI number using MPI
*
* Soroush Hashemifar - Iran University of Science and Technology (IUST), Tehran, Iran.
* 
* For 8 processes, just run this:
* mpicc -o test mpi_monte_carlo_pi.c -lm
* mpirun -np 8 ./test
* 
************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

long niter = 100000000;
int rank;                       //holds process's rank id
double x, y;                     //x,y value for the random coordinate
int i, count=0;                 //Count holds all the number of how many good coordinates
double pi;                      //holds approx value of pi
int size;
double start_time; // hold start time
double end_time; // hold end time
MPI_Status status; // store status of a MPI_Recv
MPI_Request request; //capture request of a MPI_Isend

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);                 //Start MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);           //get rank of node's process
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int recieved[size];
    srand(4);                       //Give rand() a seed value. Needs to be different on each node

    if (rank != 0) {
        for (i=0; i < niter / size; i++) {
            x = ((double) rand()) / RAND_MAX;           //gets a random x coordinate
            y = ((double) rand()) / RAND_MAX;           //gets a random y coordinate
            //Checks to see if number in inside unit circle
            if (sqrt(x*x + y*y) <= 1) {
                count++;                //if it is, consider it a valid random point
            }
        }

        MPI_Isend(&count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &request);
    }
    
    if (rank == 0) {
        start_time = MPI_Wtime();

        int finalcount = 0;

        for (i=0; i < niter / size; i++) {
            x = ((double) rand()) / RAND_MAX;           //gets a random x coordinate
            y = ((double) rand()) / RAND_MAX;           //gets a random y coordinate
            //Checks to see if number in inside unit circle
            if (sqrt(x*x + y*y) <= 1) {
                finalcount++;                //if it is, consider it a valid random point
            }
        }

        for (i=1; i < size; i++) {
            MPI_Recv(&recieved[i], 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
            finalcount += recieved[i];
        }

        pi = ((double) finalcount / (double) niter) * 4.0;               // p = 4 * (m / n)
        
        end_time = MPI_Wtime();
        printf("\nRunning Time = %f\n\n", end_time - start_time);
        printf("Pi: %f\n", pi);             //Print the calculated value of pi
 
    }
 
    MPI_Finalize();

    return 0;
}