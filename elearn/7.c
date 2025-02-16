/*Implementare un programma parallelo per l’ambiente
multicore con np unità processanti che impieghi la
libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve generare
una matrice A di dimensione N×M. Quindi, i core
devono collaborare per calcolare il minimo tra gli
elementi delle colonne di propria competenza e
conservare tale valore in un vettore c di dimensione M.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main(){

    int i, j, n, m, np;
    float **a, *c;
    double ti, tf;

    srand(time(NULL));

    printf("Inserici il numero di core: \n");
    scanf("%d", &np);

    printf("Inserici il numero di righe: \n");
    scanf("%d", &n);

    printf("Inserici il numero di colonne: \n");
    scanf("%d", &m);

    #pragma omp master
    {
        a = (float **)calloc(n, sizeof(float *));
        for(i = 0; i < n; i++){
            a[i] = (float *)calloc(m, sizeof(float));
        }

        c = (float *)calloc(m, sizeof(float));

        for(i = 0; i < n; i++){
            for(j = 0; j < m; j++){
                a[i][j] = rand()%100 - 10;
            }
        }

        printf("La matrice generata A: \n");
        for(i = 0; i < n; i++){
            for(j = 0; j < m; j++){
                printf("a[%d][%d] = %.2f\t", i, j, a[i][j]);
            }
            printf("\n");
        }
    }

    ti = omp_get_wtime();
    #pragma omp parallel for private(i, j) shared(n, m, a, c) num_threads(np)
    for(j = 0; j < m; j++){
        float min = a[0][j];
        for(i = 1; i < n; i++){
            if(a[i][j] < min){
                min = a[i][j];
            }
        }
        c[j] = min;
    }

    tf = omp_get_wtime();

    printf("Il vettore C dei minimi delle colonne e': \n");
    for(j = 0; j < m; j++){
        printf("c[%d] = %.2f\t", j, c[j]);
    }

    printf("\n");

    printf("Il tempo di esecuzione e': %f\n", tf-ti);

    for(i = 0; i < n; i++){
        free(a[i]);
    }
    free(a);
    free(c);

    return 0;

}
