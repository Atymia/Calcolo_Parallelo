/* Implementare il seguente programma:

Il core master deve generare una matrice A di dimensione N * M e
un vettore b di dimensione M;
I core devono collaborare per costruire un vettore c le cui M componenti assumono il valore del massimo di ogni colonna della matrice A sommato all'identificativo del thread;
I core collaborano per calcolare in parallelo il prodotto scalare
prod = c * b;
Il core master deve stampare il risultato finale ed il tempo di esecuzione.
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(){

    int n, m, i, j, np;
    float **A, *b, *c, max, prod = 0.0;
    double ti, tf;

    srand(time(NULL));

    printf("Inserire il numero di core: \n");
    scanf("%d", &np);

    printf("Inserire il numero di righe della matrice A: \n");
    scanf("%d", &n);

    printf("Inserire il numero di colonne della matrice A: \n");
    scanf("%d", &m);

    #pragma omp master
    {
        // allocazione della matrice A
        A = (float **)calloc(n, sizeof(float *));
        for(i = 0; i < n; i++){
            A[i] = (float *)calloc(m, sizeof(float));
        }

        // allocazione dei vettori b e c
        b = (float *)calloc(m, sizeof(float));
        c = (float *)calloc(m, sizeof(float));

        for(i = 0; i < n; i++){
            for(j = 0; j < m; j++){
                A[i][j] = rand()%10;
            }
        }

        for(i = 0; i < m; i++){
            b[i] = rand()%10;
        }

        printf("\nLa matrice A generata: \n");
        for(i = 0; i < n; i++){
            for(j = 0; j < m; j++){
                printf("A[%d][%d] = %.2f\t", i, j, A[i][j]);
            }
            printf("\n");
        }

        printf("\nIl vettore b: \n");
        for(i = 0; i < m; i++){
            printf("b[%d] = %.2f\t", i, b[i]);
        }
    }

    ti = omp_get_wtime();

    // calcolo del vettore c
    #pragma omp parallel for num_threads(np) private(i, j, max) shared(A, c, n, m)
    for(j = 0; j < m; j++){
        int id = omp_get_thread_num();
        max = A[0][j];
        for(i = 1; i < n; i++){
            if(A[i][j] > max){
                max = A[i][j];
            }
        }
        c[j] = max + id;
    }

    // calcolo del prodotto scalare c*b
    #pragma omp parallel for num_threads(np) shared(c, b, m) reduction(+:prod)
    for(i = 0; i < m; i++){
        prod += c[i] * b[i];
    }

    tf = omp_get_wtime();

    printf("\nIl vettore c: \n");
    for(i = 0; i < m; i++){
        printf("c[%d] = %.2f\t", i, c[i]);
    }
    printf("\n");

    printf("\nIl prodotto scalare e': %.2f\n", prod);
    printf("IL tempo di esecuzione: %f secondi\n", tf-ti);

    for(i = 0; i < n; i++){
        free(A[i]);
    }
    free(A);
    free(b);
    free(c);

    return 0;
}
