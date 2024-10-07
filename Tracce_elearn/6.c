/* Implementare un programma parallelo per l’ambiente
multicore con np unità processanti che impieghi la
libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve leggere
una matrice A di dimensione N×N ed uno scalare alpha.
Quindi, i core devono collaborare per calcolare il
prodotto tra lo scalare alpha e la matrice A, sfruttando
una strategia di parallelizzazione che usi la
distribuzione per colonne della matrice A come
decomposizione del dominio.*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(){

    int n, i, j;
    float **A;
    float alpha;
    double temp_i, temp_f;

    printf("Inserisci la dimensione della matrice A: ");
    scanf("%d", &n);

    srand(time(NULL));

    #pragma omp master
    {
        // Allocazione matrice
        A = (float **)calloc(n, sizeof(float *));
        for(i = 0; i < n; i++){
            A[i] = (float *)calloc(n, sizeof(float));
        }

        // Riempimento della matrice
        for(i = 0; i < n; i++){
            for(j = 0; j  < n; j++){
                A[i][j] = rand() % 10;
            }
        }

        printf("Inserisci lo scalare alpha: ");
        scanf("%f", &alpha);
    }

    printf("La matrice A e': \n");
        // Stampa la matrice A
        for(i = 0; i < n; i++){
            for(j = 0; j< n; j++) {
            printf("%.1f\t", A[i][j]);
            }
            printf("\n");
        }

    float **B = (float **)calloc(n, sizeof(float *));
    for(i = 0; i < n; i++){
        B[i] = (float *)calloc(n, sizeof(float));
    }

    temp_i = omp_get_wtime();

    #pragma omp parallel for shared(A, B, alpha) private(i, j) collapse(2)
    for(j = 0; j < n; j++){
        for(i = 0; i < n; i++){
            B[i][j] = alpha*A[i][j];
        }
    }

    temp_f = omp_get_wtime();

    // Stampa la matrice risultato B
    printf("La matrice B e': \n");
        // Stampa la matrice A
        for(i = 0; i < n; i++){
            for(j = 0; j< n; j++) {
            printf("%.1f\t", B[i][j]);
            }
            printf("\n");
        }

    double temp = temp_f - temp_i;

    printf("Il tempo impiegato e': %f\n", temp);

    // Dealloca matrice A e B
    for(i = 0; i < n; i++){
        free(A[i]);
        free(B[i]);
    }

        free(A);
        free(B);

        return 0;
}
