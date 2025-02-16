/*Implementare un programma parallelo per l'ambiente multicore con np unità processanti, utilizzando la libreria OpenMP.
Il core master legge due vettori a di dimensione N e b di dimensione M, e un terzo vettore c di dimensione M. I core collaborano per calcolare il prodotto esterno tra i vettori a e b, generando una matrice A di dimensione N×M. Successivamente, viene calcolato il prodotto tra la matrice A e il vettore c, producendo un vettore risultato Result di dimensione N. Il core master stampa i vettori, la matrice e il risultato, visualizzando anche il tempo di esecuzione complessivo del calcolo.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(){

    int i, j, n, m, np;
    float *a, *b, *c, **A, *result;
    double ti, tf;

    srand(time(NULL));

    printf("Inserire il numero di core: \n");
    scanf("%d", &np);

    printf("Inserire la dimensione del vettore a(N): \n");
    scanf("%d", &n);

    printf("Inserire la dimensione del vettore b(M) e del vettore c(M): \n");
    scanf("%d", &m);

    #pragma omp master
    {
        A = (float **)calloc(n, sizeof(float *));
        for(i = 0; i < n; i++){
            A[i] = (float *)calloc(m, sizeof(float));
        }

        a = (float *)calloc(n, sizeof(float));
        b = (float *)calloc(m, sizeof(float));
        c = (float *)calloc(m, sizeof(float));
        result = (float *)calloc(n, sizeof(float));



        for(i = 0; i < n; i++){
            a[i] = (rand()%10)+1;
        }

        for(i = 0; i < m; i++){
            b[i] = (rand()%10)+1;
            c[i] = (rand()%10)+1;
        }

        printf("\nVettore a: \n");
        for(i = 0; i < n; i++){
            printf("a[%d] = %.2f\t", i, a[i]);
        }
        printf("\n");

        printf("\nVettore b: \n");
        for(i = 0; i < m; i++){
            printf("b[%d] = %.2f\t", i, b[i]);
        }
        printf("\n");

        printf("\nVettore c: \n");
        for(i = 0; i < m; i++){
            printf("c[%d] = %.2f\t", i, c[i]);
        }
        printf("\n");
    }

    ti = omp_get_wtime();

    #pragma omp parallel for num_threads(np) private(i, j) shared(a, b, c, A, result)
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            A[i][j] = a[i]*b[j];
        }
    }

    #pragma omp parallel for num_threads(np) private(i, j) shared(A, c,  result)
    for(i = 0; i < n; i++){
        result[i] = 0.0;
        for(j = 0; j < m; j++){
            result[i] += A[i][j] * c[j];
        }
    }

    tf = omp_get_wtime();

    printf("\nMatrice A: \n");
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            printf("A[%d][%d] = %.2f\t", i, j, A[i][j]);
        }
        printf("\n");
    }

    printf("\nVettore risultato: \n");
    for(i = 0; i < n; i++){
        printf("result[%d] = %.2f\t", i, result[i]);
    }
    printf("\n");

    printf("Tempo si esecuzione: %f secondi\n", tf-ti);

    for(i = 0; i < n; i++){
        free(A[i]);
    }
    free(A);
    free(a);
    free(b);
    free(c);
    free(result);

    return 0;
}
