/* Il core master inizializza due matrici quadrate A e B di dimensione N e un vettore b di dimensione N;
I core collaborano per calcolare la somma degli elementi di ogni riga della matrice A e memorizzarla nel vettore b;
Successivamente, i core eseguono un prodotto scalare tra il vettore b e la matrice B per ottenere un nuovo vettore di risultati;
Il core master deve stampare il tempo di esecuzione, il vettore b e il vettore risultato finale;
Alla fine, tutte le strutture allocate dinamicamente vengono deallocate. */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(){

    int i, j, n, np;
    float **A, **B, *b, *prod, sum;
    double ti, tf;

    srand(time(NULL));

    printf("Inserire il numero di core: \n");
    scanf("%d", &np);

    printf("Inserire il numero di righe(N) e colonne(N): \n");
    scanf("%d", &n);

    #pragma omp master
    {
        A = (float **)calloc(n, sizeof(float *));
        B = (float **)calloc(n, sizeof(float *));
        for(i = 0; i < n; i++){
            A[i] = (float *) calloc(n, sizeof(float));
            B[i] = (float *) calloc(n, sizeof(float));
        }

        b = (float *)calloc(n, sizeof(float));
        prod = (float *)calloc(n, sizeof(float));

        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                A[i][j] = (rand()%10)+1;
                B[i][j] = (rand()%10)+1;
            }
        }

        printf("\nQuesta e' la matrice A generata: \n");
        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                printf("A[%d][%d] = %.2f\t", i, j, A[i][j]);
            }
            printf("\n");
        }

        printf("\nQuesta e' la matrice B generata: \n");
        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                printf("B[%d][%d] = %.2f\t", i, j, B[i][j]);
            }
            printf("\n");
        }
    }

    ti = omp_get_wtime();

    #pragma omp parallel num_threads(np) private(i, j) shared(A, b, n)
    for(i = 0; i < n; i++){
        sum = 0.0;
        for(j = 0; j < n; j++){
            sum += A[i][j];
        }
        b[i] = sum;
    }

    #pragma omp parallel num_threads(np) private(i, j) shared(B, b, n)
    for(i = 0; i < n; i++){
        prod[i] = 0.0;
        for(j = 0; j < n; j++){
            prod[i] += b[j]*B[i][j];
        }
    }

    tf = omp_get_wtime();

    printf("\nIl vettore b e': \n");
    for(i = 0; i < n; i++){
        printf("b[%d] = %.2f\t", i, b[i]);
    }

    printf("\nIl vettore risultato e': \n");
    for(i = 0; i < n; i++){
        printf("prod[%d] = %.2f\t", i, prod[i]);
    }

    printf("\nIl tempo di esecuzione: %f secondi\n", tf-ti);

    for(i = 0; i < n; i++){
        free(A[i]);
        free(B[i]);
    }
    free(A);
    free(B);
    free(b);
    free(prod);

    return 0;
}
