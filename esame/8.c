/*Scrivere un programma parallelo per un ambiente multicore con np unità processanti, utilizzando la libreria OpenMP. Il programma deve ssere strutturato come segue: il core master deve generare una matrice C di dimensione N x M con valori casuali. Ogni core dovra' estrarre M/np colonne della matrice C e calcolare localmente la normalizzazione degli elementi delle sottomatrici assegnate. I risultati devono essere salvati in una matrice C_norm di dimensione N x M. Infine, il core master somma gli elementi della prima riga della matrice C_norm e stampa il risultato insieme al tempo totale di esecuzione.*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main(){

    int i, j, n, m, np;
    float **c, **c_norm, sum = 0;
    double ti, tf;

    srand(time(NULL));

    printf("Inserire numero core: \n");
    scanf("%d", &np);

    printf("Inserire numero righe(N) della matrice C\n");
    scanf("%d", &n);

    printf("Inserire numero colonne(M) della matrice C\n");
    scanf("%d", &m);

    #pragma omp master
    {
        c = (float **)calloc(n, sizeof(float *));
        c_norm = (float **)calloc(n, sizeof(float *));
        for(i = 0; i < n; i++){
            c[i] = (float *)calloc(m, sizeof(float));
            c_norm[i] = (float *)calloc(m, sizeof(float));
        }

        for(i = 0; i < n; i++){
            for(j = 0; j < m; j++){
                c[i][j] = (rand()%201-100);
            }
        }

        printf("Matrice C: \n");
        for(i = 0; i < n; i++){
            for(j = 0; j < m; j++){
                printf("C[%d][%d] = %.2f\t", i, j, c[i][j]);
            }
            printf("\n");
        }
    }

    ti = omp_get_wtime();

    #pragma omp parallel for num_threads(np) private(i, j) shared(c, c_norm, n, m)
    for(j = 0; j < m; j++){
        float sum_col = 0.0;

        // calcolo della somma dei valori assoluti della colonna
        for(i = 0; i < n; i++){
            sum_col += fabs(c[i][j]);
        }

        // normalizzazione degli elementi della colonna
        for(i = 0; i < n; i++){
            c_norm[i][j] = fabs(c[i][j]) / sum_col;
        }
    }

    tf = omp_get_wtime();

    printf("Tempo di esecuzione: %f secondi\n", tf-ti);

    printf("Matrice C_norm: \n");
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            printf("C_norm[%d][%d] = %.2f\t", i, j, c_norm[i][j]);
        }
        printf("\n");
    }


    printf("\n");

    // somma degli elementi della prima riga di C_norm
    for(j = 0; j < m; j++){
        sum += c_norm[0][j];
    }

    printf("La somma della prima riga della matrice c_norm: %.2f\n", sum);

    for(i = 0; i < n; i++){
        free(c[i]);
        free(c_norm[i]);
    }
    free(c);
    free(c_norm);

    return 0;
}
