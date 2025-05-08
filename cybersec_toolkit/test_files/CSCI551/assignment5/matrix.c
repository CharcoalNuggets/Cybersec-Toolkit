#include <stdio.h>
#include <stdlib.h>

void mat_mul(int n, double a[n][n], double b[n][n], double result[n][n]){
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++) {
            result[i][j] = 0;
            for(int k = 0; k < n; k++)
                result[i][j] += a[i][k] * b[k][j];
        }
}

void mat_vec_mul(int n, double a[n][n], double v[n], double result[n]){
    for(int i = 0; i < n; i++) {
        result[i] = 0;
        for(int j = 0; j < n; j++)
            result[i] += a[i][j] * v[j];
    }
}

int main(int argc, char** argv){
    if(argc < 2){
        fprintf(stderr, "usage: ./matrix_omp [input file]\n");
        return -1;
    }

    FILE* fp = fopen(argv[1], "r");
    if(!fp){
        perror("Error opening input file");
        return -1;
    }

    int n;
    if (fscanf(fp, "%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Error: could not read matrix size\n");
        fclose(fp);
        return -1;
    }

    double m1[n][n], m2[n][n], v1[n], result_m[n][n], result_v[n];

    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            fscanf(fp, "%lf", &m1[i][j]);

    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            fscanf(fp, "%lf", &m2[i][j]);

    for(int i = 0; i < n; i++)
        fscanf(fp, "%lf", &v1[i]);

    fclose(fp);

    mat_mul(n, m1, m2, result_m);
    mat_vec_mul(n, m1, v1, result_v);

    printf("Matrix Multiplication Result (m1 * m2):\n");
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++)
            printf("%6.1f ", result_m[i][j]);
        printf("\n");
    }

    printf("Matrix-Vector Multiplication Result (m1 * v1):\n");
    for(int i = 0; i < n; i++)
        printf("%6.1f\n", result_v[i]);

    return 0;
}
