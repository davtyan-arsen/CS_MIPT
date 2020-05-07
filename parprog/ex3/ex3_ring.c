#include "stdio.h"
#include "stdlib.h"
#include "mpi.h"

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int i = 0;
    int rank = 0;
    int NP = 0; // number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &NP);

    int* data;
    data = (int*)malloc(NP * sizeof(int)); // all processes allocate memory for the array

    if (rank == 0) {
        for (i = 0; i < NP; ++i) {
            data[i] = atoi(argv[i + 1]); // main process initializes the array
        }
        printf("process %d -- %d\n", rank,  data[rank]);
    }

    if (rank != 0) {
        MPI_Recv(data, NP, MPI_INT, rank - 1, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("process %d -- %d\n", rank,  data[rank]);
        // process n sends the array to the process (n + 1)
        // process 0 doesnt receive anything
    }

    if (rank != NP - 1)
        MPI_Send(data, NP, MPI_INT, rank + 1, rank + 1, MPI_COMM_WORLD);
        // process (NP - 1) doesnt send anything

    MPI_Finalize();
    return 0;
}
