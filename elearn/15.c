/*Implementare un programma parallelo per
l’ambiente multicore con np unità processanti che
impieghi la libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve leggere un
vettore a, di dimensione M, e uno scalare alpha. Quindi
i core devono collaborare per costruire una matrice A,
di dimensione M×M, la cui diagonale principale ha per
elementi quelli del vettore a moltiplicati per lo scalare
alpha.

--Aggiunta anche della trasposta di A locale
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main(){

    int m, i, j, np;
    float *a, **A, alpha;
    double ti, tf;

    srand(time(NULL));

    printf("Inserire il numero di core: \n");
    scanf("%d", &np);

    printf("Inserire il numero di righe(M) e di colonne(M): \n");
    scanf("%d", &m);

    printf("Inserire lo scalare alpha: \n");
    scanf("%f", &alpha);

    #pragma omp master
    {
        A = (float **)calloc(m, sizeof(float *));
        for(i = 0; i < m; i++){
            A[i] = (float *)calloc(m, sizeof(float));
        }

        a = (float *)calloc(m, sizeof(float));

        for(i = 0; i < m; i++){
            for(j = 0; j < m; j++){
                A[i][j] = rand()%10;
            }
        }

        for(i = 0; i < m; i++){
            a[i] = rand()%10;
        }

        printf("Questo e' il vettore a: \n");
        for(i = 0; i < m; i++){
            printf("a[%d] = %.2f\t", i, a[i]);
        }
    }

    ti = omp_get_wtime();

    #pragma omp parallel for num_threads(np) shared(A, a, alpha, m)
    for(i = 0; i < m; i++){
        A[i][i] = a[i] * alpha;
    }

    // Stampa della matrice A originale
    printf("\nLa matrice A originale e': \n");
    for(i = 0; i < m; i++){
        for(j = 0; j < m; j++){
            printf("A[%d][%d] = %.2f\t", i, j, A[i][j]);
        }
        printf("\n");
    }

    // Calcolo della trasposta in parallelo senza usare una matrice separata
    #pragma omp parallel for num_threads(np) shared(A, m) private(i, j)
    for(i = 0; i < m; i++){
        for(j = i+1; j < m; j++){
            // Scambio A[i][j] con A[j][i]
            float temp = A[i][j];
            A[i][j] = A[j][i];
            A[j][i] = temp;
        }
    }

    tf = omp_get_wtime();

    // Stampa della matrice trasposta
    printf("\nLa matrice A trasposta e': \n");
    for(i = 0; i < m; i++){
        for(j = 0; j < m; j++){
            printf("A[%d][%d] = %.2f\t", i, j, A[i][j]);
        }
        printf("\n");
    }

    printf("Il tempo di esecuzione e': %f secondi\n", tf - ti);

    for(i = 0; i < m; i++){
        free(A[i]);
    }
    free(A);
    free(a);

    return 0;
}

