/**********************************************************************************************
* Matrix Square Root Program (Serialized)
*
* Soroush Hashemifar - Iran University of Science and Technology (IUST), Tehran, Iran.
* 
*
* For 8 processes, just run this:
* mpicc -o test mpi_sqrt_matrix.c -lm
* mpirun -np 8 ./test
* 
************************************************************************************************/

#include <stdio.h>
#include <math.h>

#define NUM_ROWS_A 8192 // rows of input [A]
#define NUM_COLUMNS_A 8192 // columns of input [A]

int i, j; // helper variables
double mat_a[NUM_ROWS_A][NUM_COLUMNS_A]; // declare input [A]
double mat_result[NUM_ROWS_A][NUM_COLUMNS_A]; // declare output [C]

int main(int argc, char *argv[]) {
    for (i = 0; i < NUM_ROWS_A; i++) {
        for (j = 0; j < NUM_COLUMNS_A; j++) {
            mat_a[i][j] = i * j;
        }
    }
    
    for (i = 0; i < NUM_COLUMNS_A; i++) {
        for (j = 0; j < NUM_ROWS_A; j++) {
            mat_result[i][j] = sqrt(mat_a[i][j]);
        }
    }
        
    return 0;
}