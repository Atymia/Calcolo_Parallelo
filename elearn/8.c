/* Implementare un programma parallelo per l’ambiente
multicore con np unità processanti che impieghi la
libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve generare
una matrice A di dimensione N×M. Quindi, ogni core
deve estrarre N/np colonne ed effettuare localmente la
somma degli elementi delle sottomatrici estratte,
conservando il risultato in un vettore b di dimensione
M.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main(){

    int i, j, n, m, np;
    float **a, *b;
    double ti, tf;

    srand(time(NULL));

    printf("Inserisci il numero di core: \n");
    scanf("%d", &np);

    printf("Inserisci il numero di righe: \n");
    scanf("%d", &n);

    printf("Inserisci il numero di colonne: \n");
    scanf("%d", &m);

    #pragma omp master
    {
        a = (float **)calloc(n, sizeof(float *));
        for(i = 0; i < n; i++){
            a[i] = (float *)calloc(m, sizeof(float));
        }

        b = (float *)calloc(m, sizeof(float));

        for(i = 0; i < n; i++){
            for(j = 0; j < m; j++){
                a[i][j] = rand()%10;
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

    #pragma omp parallel num_threads(np)
    {
        int id = omp_get_thread_num();
        int nloc = m/np;
        int start_col = id*nloc; // colonna iniziale
        int end_col = (id == np - 1)?m: start_col + nloc; // colonna finale

        for(j = start_col; j < end_col; j++){
            float sum = 0;
            for(i = 0; i < n; i++){
                sum += a[i][j];
            }
            b[j] = sum;
        }
    }

    tf = omp_get_wtime();

    printf("Il vettore B delle somme delle colonne e': \n");
    for(j = 0; j < m; j++){
        printf("b[%d] = %.2f\n", j, b[j]);
    }

    printf("Il tempo di esecuzione e': %f\n", tf-ti);

    for(i = 0; i < n; i++){
        free(a[i]);
    }
    free(a);
    free(b);

    return 0;
}
