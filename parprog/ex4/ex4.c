#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "mpi.h"

/*
    f(x) = 1/(x^2+2)^2 = sum(n=0,inf) { (-1)^n * (n+1) / 2^(n+2) * x^(2*n)} ; R = sqrt(2)

    !!! compile with -lm (for pow)
*/

// f(x) calculates the precise value of the function
double f(double x) {
    double res = 1. / pow(x*x + 2, 2);
    return res;
}

// f_n(x, n) calculates the n-th elemnt of the Taylor series in point x
double f_n(double x, int n) {
    double res = pow(x, 2*n) * (n + 1.) / pow(2, n + 2);
    return (n % 2 == 0 ? res: -res);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank = 0;
    int size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double walltime = MPI_Wtime();

    double x = atof(argv[1]); // the point in which the series are calculated
    if(x > 1) {
        if (rank == 0)
            printf("The series do not converge with this value of x\n");
        MPI_Finalize();
        return 0;
    }

    int N = atoi(argv[2]); // number of terms in Taylor series

    double sum = 0, res = 0;
    int i = 0;
    for (i = rank; i < N; i = i + size) {
        // each process calculates the sum of certain elements
        // for example if number of processes = 5 then the process with rank 2
        // calculates 2, 7, 12 .. elements
        sum += f_n(x, i);
    }

    // all those sums are sent to the main (rank = 0) process via collective communication
    MPI_Reduce(&sum, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double f1 = f(x);
        printf("f(x) = %.15f\n", f1);
        printf("calculated = %.15f\n", res);
    }

    walltime = MPI_Wtime() - walltime;
    printf("process %d - %fs \n", rank, walltime);

    MPI_Finalize();
    return 0;
}
