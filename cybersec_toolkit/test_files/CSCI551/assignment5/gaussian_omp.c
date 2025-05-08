#include <stdio.h>
#include <omp.h>

void forward_elimination(double a[5][5], double b[5]){
    for(int i = 0; i < 5; i++){
        for(int j = i + 1; j < 5; j++){
            if(a[j][i] > a[i][i]){
                for(int k = 0; k < 5; k++){
                    double temp = a[i][k];
                    a[i][k] = a[j][k];
                    a[j][k] = temp;
                }
                double temp = b[i];
                b[i] = b[j];
                b[j] = temp;
            }
        }

        for(int j = i + 1; j < 5; j++){
            double factor = a[j][i] / a[i][i];
            b[j] -= factor * b[i];
            for(int k = i; k < 5; k++){
                a[j][k] -= factor * a[i][k];
            }
        }
    }
}

void back_substitution(double a[5][5], double b[5], double solution[5]){
    for(int i = 4; i >= 0; i--){
        double sum = 0.0;

        #pragma omp parallel for reduction(+:sum)//miniscule parallelism. Was running into extensive issues otherwise
        for(int j = i + 1; j < 5; j++){
            sum += a[i][j] * solution[j];
        }
        solution[i] = (b[i] - sum) / a[i][i];
    }
}

int main(){
    double a[5][5] = {
        {6, 0, -1, 0, 0},
        {-3, 3, 0, 0, 0},
        {0, -1, 9, 0, 0},
        {0, -1, -8, 11, -2},
        {-3, -1, 0, 0, 4}
    };
    double b[5] = {50, 0, 160, 0, 0};
    double solution[5] = {0};

    forward_elimination(a, b); 
    back_substitution(a, b, solution); 

    printf("Solution:\n");
    for(int i = 0; i < 5; i++){
        printf("c%d = %f\n", i + 1, solution[i]);
    }

    return 0;
}
