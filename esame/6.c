/*Implementare un programma parallelo per un ambiente multicore con np×np unità processanti che utilizzi la libreria OpenMP. Il programma deve essere organizzato come segue:
Il core master deve generare due matrici A e B di dimensione N×N con valori casuali o determinati.
Ogni core deve calcolare il prodotto tra blocchi quadrati corrispondenti delle matrici A e B di dimensione (N/np)×(N/np).
Il risultato parziale di ogni core deve essere sommato in una matrice di risultato C di dimensione N×N.
Alla fine, il core master deve sommare tutti i risultati parziali e stampare la matrice finale C.
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(){

    int i, j, k, l, n, np, nloc;
    float **a, **b, **c;
    double ti, tf;

    srand(time(NULL));

    printf("Inserire il numero di core: \n");
    scanf("%d", &np);

    printf("Inserire la dimensione N: \n");
    scanf("%d", &n);

    nloc = n/np;

    #pragma omp master
    {
        a = (float **)calloc(n, sizeof(float *));
        b = (float **)calloc(n, sizeof(float *));
        c = (float **)calloc(n, sizeof(float *));
        for(i = 0; i < n; i++){
            a[i] = (float *)calloc(n, sizeof(float));
            b[i] = (float *)calloc(n, sizeof(float));
            c[i] = (float *)calloc(n, sizeof(float));
        }

        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                a[i][j] = rand()%10;
                b[i][j] = rand()%10;
            }
        }

        printf("La matrice A generata: \n");
        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                printf("A[%d][%d] = %.2f\t", i, j, a[i][j]);
            }
            printf("\n");
        }

        printf("\nLa matrice B generata: \n");
        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                printf("B[%d][%d] = %.2f\t", i, j, b[i][j]);
            }
            printf("\n");
        }
    }

    ti = omp_get_wtime();

    #pragma omp parallel for num_threads(np*np) private(i, j, k, l) shared(a, b, c, n, nloc)
    for(i = 0; i < np; i++){
        for(j = 0; j < np; j++){
            for(k = i*nloc; k < (i+1)*nloc; k++){
                for(l = j*nloc; l < (j+1)*nloc; l++){
                    c[k][l] = a[k][l] * b[k][l];
                }
            }
        }
    }

    tf = omp_get_wtime();

    printf("\nLa matrice C risultante: \n");
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            printf("C[%d][%d] = %.2f\t", i, j, c[i][j]);
        }
        printf("\n");
    }

    printf("\nIl tempo di esecuzione: %f secondi\n", tf-ti);

    for(i = 0; i < n; i++){
        free(a[i]);
        free(b[i]);
        free(c[i]);
    }
    free(a);
    free(b);
    free(c);

    return 0;
}
