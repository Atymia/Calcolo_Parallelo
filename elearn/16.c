/* Implementare un programma parallelo per
l’ambiente multicore con nq×np unità processanti che
impieghi la libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve costruire
una matrice A, di dimensione M×N, i cui blocchi sono
generati casualmente e in parallelo da ogni core.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main(){

    int m, n, i, j, np;
    float **a;
    double ti, tf;

    srand(time(NULL));

    printf("Inserire il numero di core: \n");
    scanf("%d", &np);

    printf("Inserire il numero di righe(M): \n");
    scanf("%d", &m);

    printf("Inserire il numero di colonne(N): \n");
    scanf("%d", &n);

    ti = omp_get_wtime();

    #pragma omp master
    {
        a = (float **)calloc(m, sizeof(float *));
        for(i = 0; i < m; i++){
            a[i] = (float *)calloc(n, sizeof(float));
        }
    }

    #pragma omp parallel for num_threads(np) shared(a, m, n)
    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
            a[i][j] = rand()%10;
        }
    }

    tf = omp_get_wtime();

    printf("Questa la matrice generata A: \n");
    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
            printf("A[%d][%d] = %.2f\t", i, j, a[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    printf("Questo e' il tempo di esecuzione: %f secondi\n");

    for(i = 0; i < n; i++){
        free(a[i]);
    }
    free(a);

    return 0;
}
