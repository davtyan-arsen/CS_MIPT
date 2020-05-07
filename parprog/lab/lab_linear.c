#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"

double f_t_x(double t, double x) {
    return (t * cos(x));
}

double InitConditionX(double x) {
    return (sin(x) + x*x);
}

double InitConditionT(double t) {
    return (t*t);
}

double ExplicitLeftCorner(double u_k_m_1, double u_k_m, double f_k_m, double tau, double h) {
    double u_k1_m = (f_k_m - (u_k_m - u_k_m_1) / h) * tau + u_k_m;
    return u_k1_m;
}

double Cross(double u_k_1_m, double u_k_m_1, double u_k_m, double u_k_m1, double f_k_m, double tau, double h) {
    double u_k1_m = (f_k_m - (u_k_m1 - u_k_m_1) / (2 * h)) * 2 * tau + u_k_1_m;
    return u_k1_m;
}

int main(int argc, char** argv) {

    clock_t begin = clock();

    double T = atof(argv[1]);
    double X = atof(argv[2]);
    int K = atoi(argv[3]);
    int M = atoi(argv[4]);
    double tau = T / K;
    double h = X / M;

    int i, j;
    double* u[K + 1];

    for (i = 0; i <= K; ++i) {
        u[i] = (double*)malloc((M + 1) * sizeof(double));
    }

    for (i = 0; i <= K; ++i) {
        u[i][0] = InitConditionT(tau * i);
    }

    for (j = 0; j <= M; ++j) {
        u[0][j] = InitConditionX(h * j);
    }

    double f_k_m;
    for (i = 1; i <= K; ++i) {
        for (j = 1; j <= M; ++j) {

            f_k_m = f_t_x((i - 1) * tau, j * h);

            if (i == 1 || j == M) {
                u[i][j] = ExplicitLeftCorner(u[i - 1][j - 1], u[i - 1][j], f_k_m, tau, h);
            }
            else {
                u[i][j] = Cross(u[i - 2][j], u[i - 1][j - 1], u[i - 1][j], u[i - 1][j + 1], f_k_m, tau, h);
            }
        }
    }
    /*
    FILE * file;
    file = fopen("lab_linear.txt","w");
    for(i = 0; i <= K; ++i){
        for(j = 0; j <= M; ++j){
            fprintf(file, "%0.5f   ", u[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose (file);
    */
    clock_t end = clock();
    double walltime = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("walltime - %fs \n", walltime);

    return 0;
}
