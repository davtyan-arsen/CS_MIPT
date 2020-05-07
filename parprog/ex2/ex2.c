#include "stdio.h"
#include "stdlib.h"
#include "mpi.h"

typedef struct {
int start;
int end;
int sum;
} T_SUM;

int main(int argc, char** argv) {
    int N = atoi(argv[1]); // counting numbers from 1 to N

    MPI_Init(&argc, &argv);

    int i = 0;
    int rank = 0;
    int NP = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &NP);
    NP -= 1; // number of processes, except the main one
    if (rank == 0) {
        // main process
        double walltime = MPI_Wtime();

        // creating array of structures to send to child processes
        int rangeLen = N / NP;
        T_SUM* data;
        data = (T_SUM*)malloc(NP * sizeof(T_SUM));
        for (i = 0; i < NP; ++i) {
            data[i].start = i * rangeLen + 1;
            data[i].end = (i + 1) * rangeLen;
            data[i].sum = 0;
        }
        data[NP - 1].end = N; // in case (N % NP) != 0
        for (i = 1; i <= NP; ++i) {
            // Sending 1 structure (3 ints) to each child process
            MPI_Send(&data[i-1], 3, MPI_INT, i, i, MPI_COMM_WORLD);
        }
        for (i = 1; i <= NP; ++i) {
            // Receiving same structures but with sum already calculated
            MPI_Recv(&data[i-1], 3, MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        int res = 0;
        for (i = 0; i < NP; ++i) {
            // summing the sums of all segments
            res += data[i].sum;
        }

        walltime = MPI_Wtime() - walltime;
        printf("%d\n", res);
        printf("main process - %fs \n", walltime);
    }
    else {
        double walltime = MPI_Wtime();

        // Receiving a structure (3 ints)
        T_SUM segment;
        MPI_Recv(&segment, 3, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (i = segment.start; i <= segment.end; ++i) {
            segment.sum += i;
        }
        // Sending the same structure after calculating sum of that segment
        MPI_Send(&segment, 3, MPI_INT, 0, rank, MPI_COMM_WORLD);

        walltime = MPI_Wtime() - walltime;
        printf("process %d - %fs \n", rank, walltime);
    }

    MPI_Finalize();
    return 0;
}
