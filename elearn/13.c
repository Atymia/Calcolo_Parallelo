/* Implementare un programma parallelo per
l’ambiente multicore con np unità processanti che
impieghi la libreria OpenMP. Il programma deve essere
organizzato come segue: il core master generare una
matrice A di dimensione M×M. Quindi, i core devono
collaborare per valutare il minimo valore tra gli elementi di
A e successivamente costruire la matrice B i cui
elementi sono dati dalla somma di tale valore minimo
per gli elementi di posto corrispondente della matrice
di partenza.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main(){

    int i, j, m, np, min;
    float **a, **b;
    double ti, tf;

    srand(time(NULL));

    printf("Inserire il numero di core: \n");
    scanf("%d", &np);

    printf("Inserire il numero di righe(M) e di colonne(M) della matrice A: \n");
    scanf("%d", &m);

    #pragma omp master
    {
        a = (float **)calloc(m, sizeof(float *));
        b = (float **)calloc(m, sizeof(float *));
        for(i = 0; i < m; i++){
            a[i] = (float *)calloc(m, sizeof(float));
            b[i] = (float *)calloc(m, sizeof(float));
        }

        for(i = 0; i < m; i++){
            for(j = 0; j < m; j++){
                a[i][j] = (rand()%10)+4;
            }
        }

        printf("Questa e' la matrice A: \n");
        for(i = 0; i < m; i++){
            for(j = 0; j < m; j++){
                printf("A[%d][%d] = %.2f\t", i, j, a[i][j]);
            }
            printf("\n");
        }
    }

    ti = omp_get_wtime();

    #pragma omp parallel for reduction(min:min) private(i, j) shared(a, m) num_threads(np)
    for(i = 0; i < m; i++){
        min = a[0][0];
        for(j = 0; j < m; j++){
            if(a[i][j] < min){
                min = a[i][j];
            }
        }
    }

    #pragma omp parallel for private(j) shared(a, b, min, m) num_threads(np)
    for(i = 0; i < m; i++){
        for(j = 0; j < m; j++){
            b[i][j] = a[i][j] + min;
        }
    }

    tf = omp_get_wtime();

    printf("Questa e' la matrice B: \n");
    for(i = 0; i < m; i++){
        for(j = 0; j < m; j++){
            printf("B[%d][%d] = %.2f\t", i, j, b[i][j]);
        }
        printf("\n");
    }

    printf("Questo e' il tempo impiegato: %f secondi\n", tf-ti);

    for(i = 0; i < m; i++){
        free(a[i]);
        free(b[i]);
    }
    free(a);
    free(b);

    return 0;
}
