/**********************************************************************************************
* Matrix Square Root Program using MPI
*
* Soroush Hashemifar - Iran University of Science and Technology (IUST), Tehran, Iran.
* 
* Master process initializes the multiplication operands, distributes the sqrt of rows 
* operation to worker processes and reduces the worker results to construct the final output.
*  
*
* For 8 processes, just run this:
* mpicc -o test mpi_sqrt_matrix.c -lm
* mpirun -np 8 ./test
* 
************************************************************************************************/

#include <mpi.h>
#include <stdio.h>
#include <math.h>

#define NUM_ROWS_A 8192 // rows of input [A]
#define NUM_COLUMNS_A 8192 // columns of input [A]
#define MASTER_TO_SLAVE_TAG 1 // tag for messages sent from master to slaves
#define SLAVE_TO_MASTER_TAG 4 // tag for messages sent from slaves to master

void printArray(); // print the content of output matrix [C];

int rank; // process rank
int size; // number of processes
int i, j, k; // helper variables
double mat_a[NUM_ROWS_A][NUM_COLUMNS_A]; // declare input [A]
double mat_result[NUM_ROWS_A][NUM_COLUMNS_A]; // declare output [C]
double start_time; // hold start time
double end_time; // hold end time
int low_bound; //low bound of the number of rows of [A] allocated to a slave
int upper_bound; //upper bound of the number of rows of [A] allocated to a slave
int portion; //portion of the number of rows of [A] allocated to a slave
MPI_Status status; // store status of a MPI_Recv
MPI_Request request; //capture request of a MPI_Isend

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv); // initialize MPI operations
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // get the rank
    MPI_Comm_size(MPI_COMM_WORLD, &size); // get number of processes

    /* master initializes work */
    if (rank == 0) {
        for (i = 0; i < NUM_ROWS_A; i++) {
            for (j = 0; j < NUM_COLUMNS_A; j++) {
                mat_a[i][j] = i * j;
            }
        }

        start_time = MPI_Wtime();

        for (i = 1; i < size; i++) { // for each slave other than the master
            portion = NUM_ROWS_A / (size - 1); // calculate portion without master
            low_bound = (i - 1) * portion;
            
            if (((i + 1) == size) && ((NUM_ROWS_A % (size - 1)) != 0)) { // if rows of [A] cannot be equally divided among slaves
                upper_bound = NUM_ROWS_A; // last slave gets all the remaining rows
            } else {
                upper_bound = low_bound + portion; // rows of [A] are equally divisable among slaves
            }

            //send the low bound first without blocking, to the intended slave
            MPI_Isend(&low_bound, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD, &request);
            //next send the upper bound without blocking, to the intended slave
            MPI_Isend(&upper_bound, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG + 1, MPI_COMM_WORLD, &request);
            //finally send the allocated row portion of [A] without blocking, to the intended slave
            MPI_Isend(&mat_a[low_bound][0], (upper_bound - low_bound) * NUM_COLUMNS_A, MPI_DOUBLE, i, MASTER_TO_SLAVE_TAG + 2, MPI_COMM_WORLD, &request);
        }
    }
    
    /* work done by slaves */
    if (rank > 0) {
        //receive low bound from the master
        MPI_Recv(&low_bound, 1, MPI_INT, 0, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD, &status);
        //next receive upper bound from the master
        MPI_Recv(&upper_bound, 1, MPI_INT, 0, MASTER_TO_SLAVE_TAG + 1, MPI_COMM_WORLD, &status);
        //finally receive row portion of [A] to be processed from the master
        MPI_Recv(&mat_a[low_bound][0], (upper_bound - low_bound) * NUM_COLUMNS_A, MPI_DOUBLE, 0, MASTER_TO_SLAVE_TAG + 2, MPI_COMM_WORLD, &status);

        for (i = low_bound; i < upper_bound; i++) {
            for (j = 0; j < NUM_COLUMNS_A; j++) {
                mat_result[i][j] = sqrt(mat_a[i][j]);
            }
        }
        
        //send back the low bound first without blocking, to the master
        MPI_Isend(&low_bound, 1, MPI_INT, 0, SLAVE_TO_MASTER_TAG, MPI_COMM_WORLD, &request);
        //send the upper bound next without blocking, to the master
        MPI_Isend(&upper_bound, 1, MPI_INT, 0, SLAVE_TO_MASTER_TAG + 1, MPI_COMM_WORLD, &request);
        //finally send the processed portion of data without blocking, to the master
        MPI_Isend(&mat_result[low_bound][0], (upper_bound - low_bound) * NUM_COLUMNS_A, MPI_DOUBLE, 0, SLAVE_TO_MASTER_TAG + 2, MPI_COMM_WORLD, &request);
    }

    /* master gathers processed work */
    if (rank == 0) {
        for (i = 1; i < size; i++) { // untill all slaves have handed back the processed data
            //receive low bound from a slave
            MPI_Recv(&low_bound, 1, MPI_INT, i, SLAVE_TO_MASTER_TAG, MPI_COMM_WORLD, &status);
            //receive upper bound from a slave
            MPI_Recv(&upper_bound, 1, MPI_INT, i, SLAVE_TO_MASTER_TAG + 1, MPI_COMM_WORLD, &status);
            //receive processed data from a slave
            MPI_Recv(&mat_result[low_bound][0], (upper_bound - low_bound) * NUM_COLUMNS_A, MPI_DOUBLE, i, SLAVE_TO_MASTER_TAG + 2, MPI_COMM_WORLD, &status);
        }
        
        end_time = MPI_Wtime();
        
        printf("\nRunning Time = %f\n\n", end_time - start_time);
        // printArray();
    }
    
    MPI_Finalize(); // finalize MPI operations

    return 0;
}

void printArray() {
    for (i = 0; i < NUM_ROWS_A; i++) {
        printf("\n");
        for (j = 0; j < NUM_COLUMNS_A; j++)
            printf("%8.2f  ", mat_a[i][j]);
    }
    
    printf("\n\n\n");

    for (i = 0; i < NUM_ROWS_A; i++) {
        printf("\n");
        for (j = 0; j < NUM_COLUMNS_A; j++)
            printf("%8.2f  ", mat_result[i][j]);
    }

    printf("\n\n");
}
