/*Implementare un programma parallelo per l’ambiente
multicore con np unità processanti che impieghi la
libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve generare
due vettori casuali di dimensione N. Ogni core deve
sommare ad ogni elemento dei due vettori il proprio
identificativo e collaborare alla costruzione di una
matrice, di dimensione N×M, con M=2np, le cui
colonne, a due a due, sono i due vettori ottenuti nella
fase precedente.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main(){

    int n, m, i, j, np;
    float **a, *b, *c;
    double ti, tf;

    srand(time(NULL));

    printf("Inserisci il numero di core: \n");
    scanf("%d", &np);

    printf("Inserici la grandezza N: \n");
    scanf("%d", &n);

    m = 2*np;

    #pragma omp master
    {
        a = (float **)calloc(n, sizeof(float *));
        for(i = 0; i < n; i++){
            a[i] = (float *)calloc(m, sizeof(float));
        }

        b = (float *)calloc(n, sizeof(float));
        c = (float *)calloc(n, sizeof(float));

        for(i = 0; i < n; i++){
            b[i] = rand()%10;
            c[i] = rand()%10;
        }

        printf("Il vettore b e': \n");
        for(i = 0; i < n; i++){
            printf("b[%d] = %.2f\t", i, b[i]);
        }

        printf("\n");

        printf("Il vettore c e': \n");
        for(i = 0; i < n; i++){
            printf("c[%d] = %.2f\t", i, c[i]);
        }

        printf("\n");
    }

    ti = omp_get_wtime();

    #pragma omp parallel for private (i, j) shared(n, m, a, b, c) num_threads(np)
    for(j = 0; j < m; j++){
        int id = omp_get_thread_num();
        for(i = 0; i < n; i++){
            if(j % 2 == 0){
                a[i][j] = b[i] + id;
            }else{
                a[i][j] = c[i] + id;
            }
        }
    }

    tf = omp_get_wtime();

    printf("La matrice A e': \n");
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            printf("A[%d][%d] = %.2f\t", i, j, a[i][j]);
        }
        printf("\n");
    }

    printf("Tempo di esecuzione: %f secondi\n", tf-ti);

    for(i = 0; i < n; i++){
        free(a[i]);
    }
    free(a);
    free(b);
    free(c);

    return 0;
}
