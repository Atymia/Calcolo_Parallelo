/* Implementare un programma parallelo per l’ambiente
multicore con np unità processanti che impieghi la
libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve leggere
una matrice A di dimensione N×N ed uno scalare alpha.
Quindi, i core devono collaborare per calcolare il
prodotto tra lo scalare alpha e la matrice A, sfruttando
una strategia di parallelizzazione che usi la
distribuzione per colonne della matrice A come
decomposizione del dominio.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main(){

    int i, j, n, np;
    float **a, alpha;
    double ti, tf;

    srand(time(NULL));

    printf("Inserisci il numero di core: \n");
    scanf("%d", &np);

    printf("Inserisci il numero di righe(N) e di colonne(N): \n");
    scanf("%d", &n);

    printf("Inserisci il valore dello scalare alpha: \n");
    scanf("%f", &alpha);

    #pragma omp master
    {
        a = (float **)calloc(n, sizeof(float *));
        for(i = 0; i < n; i++){
            a[i] = (float *)calloc(n, sizeof(float));
        }

        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                a[i][j] = rand()%10;
            }
        }

        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                printf("a[%d][%d] = %.2f\t", i, j, a[i][j]);
            }
            printf("\n");
        }
    }

    printf("\n");

    ti = omp_get_wtime();

    #pragma omp parallel for private(i, j) shared(n, a, alpha) num_threads(np)
    for(j = 0; j < n; j++){
        for(i = 0; i < n; i++){
            a[i][j] *= alpha;
        }
    }

    tf = omp_get_wtime();

    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            printf("a[%d][%d] = %.2f\t", i, j, a[i][j]);
        }
        printf("\n");
    }

    printf("Il tempo di esecuzione e': %f\n", tf-ti);

    for(i = 0; i < n; i++){
        free(a[i]);
    }
    free(a);

    return 0;
}
