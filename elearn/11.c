/*Implementare un programma parallelo per
l’ambiente multicore con np×np unità processanti che
impieghi la libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve generare
una matrice A di dimensione N×N. Quindi, ogni core
deve sommare tra loro gli elementi di un blocco
quadrato della matrice A di dimensione (N/np)×(N/np),
conservando i valori in un vettore b.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main(){

    int i, j, k, l, n, np;
    float **a, *b;
    double ti, tf;

    srand(time(NULL));

    printf("Inserisci il numero di core: \n");
    scanf("%d", &np);

    printf("Inserisci la grandezza N: \n");
    scanf("%d", &n);

    int nloc = n/np;

    #pragma omp master
    {
        a = (float **)calloc(n, sizeof(float *));
        for(i = 0; i < n; i++){
            a[i] = (float *)calloc(n, sizeof(float));
        }

        b = (float *)calloc(np*np, sizeof(float));

        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                a[i][j] = rand()%10;
            }
        }

        printf("Questa e' la matrice A: \n");
        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                printf("A[%d][%d] = %.2f\t", i, j, a[i][j]);
            }
            printf("\n");
        }
    }

    ti = omp_get_wtime();

    #pragma omp parallel for private(i, j, k, l) shared(a, b, nloc, n, np) num_threads(np*np)
    for(i = 0; i < np; i++){
        for(j = 0; j < np; j++){
            int block_id = i*np+j; // id del singolo blocco, i*np detemrina in quale riga della matrice si trova il blocco, +j determina in quale colonna si trova il blocco
            float block_sum = 0.0;

            // somma elementi del blocco
            for(k = i * nloc; k < (i + 1) * nloc; k++){
                for(l = j * nloc; l < (j + 1) * nloc; l++){
                    block_sum += a[k][l];
                }
            }
            b[block_id] = block_sum;
        }
    }

    tf = omp_get_wtime();

    printf("Risultato somma blocchi: \n");
    for(i = 0; i < np*np; i++){
        printf("b[%d] = %.2f\t", i, b[i]);
    }

    printf("\n");

    printf("Il tempo di esecuzione e': %f secondi\n", tf-ti);

    for(i = 0; i < n; i++){
        free(a[i]);
    }
    free(a);
    free(b);

    return 0;
}
