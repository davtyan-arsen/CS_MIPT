#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "mpi.h"

// non-linearity function
double f_t_x(double t, double x) {
    return (t * cos(x));
}

// initial condition on axis x
double InitConditionX(double x) {
    return (sin(x) + x*x);
}

// initial condition on axis t
double InitConditionT(double t) {
    return (t*t);
}

// function calculates the upper layer u_k+1_m given u_k_m-1, u_k_m, f_k_m, tau and h
// using the explicit left corner template
double ExplicitLeftCorner(double u_k_m_1, double u_k_m, double f_k_m, double tau, double h) {
    double u_k1_m = (f_k_m - (u_k_m - u_k_m_1) / h) * tau + u_k_m;
    return u_k1_m;
}

// function calculates the upper layer u_k+1_m given u_k-1_m, u_k_m-1, u_k_m, u_k_m+1 , f_k_m, tau and h
// using the cross template
double Cross(double u_k_1_m, double u_k_m_1, double u_k_m, double u_k_m1, double f_k_m, double tau, double h) {
    double u_k1_m = (f_k_m - (u_k_m1 - u_k_m_1) / (2 * h)) * 2 * tau + u_k_1_m;
    return u_k1_m;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    double walltime = MPI_Wtime();

    // getting all parameters
    double T = atof(argv[1]);
    double X = atof(argv[2]);
    int K = atoi(argv[3]);
    int M = atoi(argv[4]);
    double tau = T / K;
    double h = X / M;

    int rank, size, i, j, k;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Calculating the bounds of mesh for each process
    int step = round( ((double)M + 1) / size );
    int leftBound = M - (size - rank) * step + 1;
    int rightBound = M - (size - rank - 1) * step;

    if (rank == 0) {
        leftBound = 0;
    }

    double* u[K + 1];
    int range = rightBound - leftBound;

    // each process allocates as much memory as it needs
    // process 0 allocates memory for the whole mesh so that it can later fill it in
    for (i = 0; i <= K; ++i) {
        u[i] = (double*)malloc((rank == 0 ? M + 1 : step) * sizeof(double));
    }

    // processes initialize the condition on the first layer
    for (j = 0; j <= range; ++j) {
        u[0][j] = InitConditionX((leftBound + j) * h);
    }

    double f_k_m, u_k_m_minus, u_k_m_plus;

    for (i = 1; i <= K; ++i) {
        if(rank != size - 1) {
            // if the current process isn't the last one then it sends the rightmost value
            // from the previous layer to the next process and receives its leftmost value
            MPI_Send(&u[i - 1][range], 1, MPI_DOUBLE, rank + 1, rank + 1, MPI_COMM_WORLD);
            MPI_Recv(&u_k_m_plus, 1, MPI_DOUBLE, rank + 1, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        if (rank != 0) {
            // if the current process isn't the first one then it sends the leftmost value
            // from the previous layer to the previous process and receives its rightmost value
            MPI_Send(&u[i - 1][0], 1, MPI_DOUBLE, rank - 1, rank - 1, MPI_COMM_WORLD);
            MPI_Recv(&u_k_m_minus, 1, MPI_DOUBLE, rank - 1, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // in the second layer only explicit layer should be used
        if (i == 1) {
            for (j = 0; j <= range; ++j) {
                f_k_m = f_t_x((i - 1) * tau, (leftBound + j) * h);
                if (j == 0) {
                    if (rank == 0) {
                        u[i][j] = InitConditionT(i * tau);
                    }
                    else {
                        // if it is the leftmost value that should be calculated then
                        // the rightmost value from the previous process is used in the template
                        u[i][j] = ExplicitLeftCorner(u_k_m_minus, u[i - 1][j], f_k_m, tau, h);
                    }
                }
                else {
                    u[i][j] = ExplicitLeftCorner(u[i - 1][j - 1], u[i - 1][j], f_k_m, tau, h);
                }
            }
        }
        else {
            for (j = 0; j <= range; ++j) {
                f_k_m = f_t_x((i - 1) * tau, (leftBound + j) * h);
                if (j == 0) {
                    if (rank == 0) {
                        u[i][j] = InitConditionT(i * tau);
                    }
                    else {
                        // if it is the leftmost value that should be calculated then
                        // the rightmost value from the previous process is used in the template
                        u[i][j] = Cross(u[i - 2][j], u_k_m_minus, u[i - 1][j], u[i - 1][j + 1], f_k_m, tau, h);
                    }
                }
                else if (j == range) {
                    if (rank == size - 1) {
                        // the last process always uses the explicit left corner template
                        // to calculate its rightmost value
                        u[i][j] = ExplicitLeftCorner(u[i - 1][j - 1], u[i - 1][j], f_k_m, tau, h);
                    }
                    else {
                        // if it is the rightmost value that should be calculated then
                        // the leftmost value from the next process is used in the template
                        u[i][j] = Cross(u[i - 2][j], u[i - 1][j - 1], u[i - 1][j], u_k_m_plus, f_k_m, tau, h);
                    }
                }
                else {
                    u[i][j] = Cross(u[i - 2][j], u[i - 1][j - 1], u[i - 1][j], u[i - 1][j + 1], f_k_m, tau, h);
                }
            }
        }
    }

    if (rank == 0) {
        for (k = 1; k <= size - 1; ++k) {
            // after the last layer has been calculated, process 0 receives the results from other ones
            leftBound = rightBound + 1 + step * (k - 1);
            for (i = 0; i <= K; ++i) {
                MPI_Recv(&u[i][leftBound], step, MPI_DOUBLE, k, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }

        walltime = MPI_Wtime() - walltime;
        printf("walltime - %fs \n", walltime);

        /*
        // writing the results into a file if it is necessary
        FILE * file;
        file = fopen("lab.txt","w");
        for(i = 0; i <= K; ++i){
            for(j = 0; j <= M; ++j){
                fprintf(file, "%0.5f   ", u[i][j]);
            }
            fprintf(file, "\n");
        }
        fclose (file);
        */

    }
    else {
        // After all processes calculated the values of their segments
        // they send them to the process 0
        for (i = 0; i <= K; ++i) {
            MPI_Send(&u[i][0], step, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
