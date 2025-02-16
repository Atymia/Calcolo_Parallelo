/*Genera una matrice A di dimensioni n×m con elementi casuali. Utilizzando i thread, calcola in parallelo la norma di Frobenius di A.
Stampa il risultato e il tempo totale di esecuzione.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main(){

    int i, j, m, n, np;
    float **a, norma_f;
    double ti, tf;

    srand(time(NULL));

    printf("Inserire il numero di core: \n");
    scanf("%d", &np);

    printf("Inserire il numero di righe(N) della matrice A:\n");
    scanf("%d", &n);

    printf("Inserire il numero di colonne(M) della matrice A:\n");
    scanf("%d", &m);

    #pragma omp master
    {
        a = (float **)calloc(n, sizeof(float *));
        for(i = 0; i < n; i++){
            a[i] = (float *)calloc(m, sizeof(float));
        }

        for(i = 0; i < n; i++){
            for(j = 0; j < m; j++){
                a[i][j] = rand()%10;
            }
        }

        printf("Matrice A:\n");
        for(i = 0; i < n; i++){
            for(j = 0; j < m; j++){
                printf("A[%d][%d] = %.2f\t", i, j, a[i][j]);
            }
            printf("\n");
        }
    }

    ti = omp_get_wtime();

    #pragma omp parallel for num_threads(np) private(i, j) shared(n, m, a) reduction(+:norma_f)
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            norma_f += pow(a[i][j], 2);
        }
    }
    norma_f = sqrt(norma_f);
    tf = omp_get_wtime();

    printf("La norma di Frobenius della matrice A e': %.2f\n", norma_f);
    printf("\nIl tmepo di esecuzione: %f secondi\n", tf-ti);

    for(i = 0; i < n; i++){
        free(a[i]);
    }
    free(a);

    return 0;
}
