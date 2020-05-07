#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "mpi.h"

/*
    Leibniz formula
    pi = 4 * (1 - 1/3 + 1/5 - 1/7 + 1/9 - ...)

    pi = 2*sqrt(3) * sum(n=0;inf) { (-1)^n / 3^n / (2*n+1)}

    !!! compile with -lm (for pow)
*/

// The logic is similar to ex4.c

double leibniz_n(int n) {
    return n % 2 == 0 ? 4. / (2*n+1) : -4. / (2*n+1);
}

double pi_elem_n(int n) {
    double res = 1. / pow(3, n) / (2*n + 1);
    return n % 2 == 0 ? res : -res;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    double walltime = MPI_Wtime();

    int N = atoi(argv[1]); // number of terms in Taylor series

    double sum = 0, sum_l = 0, res = 0, res_l = 0;

    int i = 0;
    int rank = 0;
    int size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (i = rank; i < N; i = i + size) {
        sum += pi_elem_n(i);
        sum_l += leibniz_n(i);
    }

    MPI_Reduce(&sum, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sum_l, &res_l, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


    if (rank == 0) {
        double pi = 2 * sqrt(3) * res;
        printf("pi = %.10f\n", pi);
        printf("pi (leibniz) = %.10f\n", res_l);
    }

    walltime = MPI_Wtime() - walltime;
    printf("process %d - %fs \n", rank, walltime);

    MPI_Finalize();
    return 0;
}
