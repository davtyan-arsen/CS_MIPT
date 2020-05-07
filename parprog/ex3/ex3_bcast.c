#include "stdio.h"
#include "stdlib.h"
#include "mpi.h"

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int NP = argc - 1; // number of processes
    int* data;
    data = (int*)malloc(NP * sizeof(int));

    int i = 0;
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        // main process
        for (i = 0; i < NP; ++i) {
            data[i] = atoi(argv[i + 1]);
        }
    }

    MPI_Bcast(data, NP, MPI_INT, 0, MPI_COMM_WORLD);

    printf("process %d -- %d\n", rank, data[rank]);

    MPI_Finalize();
    return 0;
}
