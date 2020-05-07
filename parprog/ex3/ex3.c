#include "stdio.h"
#include "stdlib.h"
#include "mpi.h"

// There is also ex3_ring.c with ring topology

// Command line arguments are the values of the array
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int i = 0;
    int rank = 0;
    int NP = 0; // number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &NP);

    // All processes create allocate memory to store the array
    int* data;
    data = (int*)malloc(NP * sizeof(int));

    if (rank == 0) {
        // main process
        for (i = 0; i < NP; ++i) {
            // main (central) process initializes the array
            data[i] = atoi(argv[i + 1]);
        }

        for (i = 1; i < NP; ++i) {
            MPI_Send(data, NP, MPI_INT, i, i, MPI_COMM_WORLD);
        }

        /*
            Could use

            MPI_Bcast(data, NP, MPI_INT, 0, MPI_COMM_WORLD);

            and there will be no need to call MPI_Send and MPI_Recv
        */

        printf("process %d -- %d\n", rank, data[rank]);

        int response = 0;
        for (i = 1; i < NP; ++i) {
            MPI_Recv(&response, 1, MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (response != 1) {
                printf("Something went wrong\n");
                MPI_Finalize();
                return -1;
            }
        }

        printf("Everything went okay\n");

    }
    else {
        // Child processes receive the array and print the value that
        // coresponds to the index equal to their rank
        MPI_Recv(data, NP, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("process %d -- %d\n", rank,  data[rank]);

        int response = 1;
        MPI_Send(&response, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
