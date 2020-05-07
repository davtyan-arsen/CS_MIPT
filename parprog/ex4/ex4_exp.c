#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "mpi.h"
#include "float.h"

/*
    exp(x) = 1 + x/1! + x^2 /2! + ... = sum(n=0;inf) { x^n /n! } ; R = inf

    !!! compile with -lm (for pow)
*/

unsigned long long factorial (int n) {
    unsigned long long res = 1;
    // I think C99 (on cluster) doesn't allow variable initialization in loop statements
    int i = 0;
    for (i = 1; i <= n; ++i) {
        res *= i;
    }
    return res;
}

// exp_n(x, n) calculates the n-th elemnt of the Taylor series in point x
double exp_n(double x, int n) {
    unsigned long long fact = factorial(n);
    if (fact < 0) {
        return 0;
    }
    double res = pow(x, n) / fact;
    if (!(res >= -DBL_MAX && res <= DBL_MAX)) {
        // starting from a certain value of n and provided |x| > 1
        // pow(x, n) gets out of bounds and becomes INFINITY
        // also dividing by INFINITY sometimes results into NAN
        return 0;
    }
    return res;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank = 0;
    int NP = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &NP);

    double walltime = MPI_Wtime();

    double x = atof(argv[1]); // the point in which the series are calculated
    int N = atoi(argv[2]); // number of terms in Taylor series

    double res = 0;

    // Each process calculates the sum of elements from start to end
    int start = rank * N / NP;
    int end = (rank + 1) * N / NP - 1;

    double sum = exp_n(x, start);
    double elem = sum;
    int i = 0;
    for (i = start + 1; i <= end; ++i) {
        elem *= x / i;
        sum += elem;
    }

    // all those sums are sent to the main (rank = 0) process via collective communication
    MPI_Reduce(&sum, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("x = %f\n", x);
        printf("exp(x) = %.10f\n", exp(x));
        printf("calculated exp(x) = %.10f\n", res);
    }

    walltime = MPI_Wtime() - walltime;
    printf("process %d - %fs \n", rank, walltime);

    MPI_Finalize();
    return 0;
}
