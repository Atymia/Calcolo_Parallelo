/* Implementare un programma parallelo per l’ambiente
multicore con np unità processanti che impieghi la
libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve generare
una matrice di dimensione N×N, ogni core deve
estrarre N/np righe e calcolare il prodotto puntuale tra
i vettori corrispondenti alle righe estratte.
*/

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));

    int rows;
    printf("Inserisci il numero di righe (N): ");
    scanf("%d", &rows);
    int cols = rows;

    // Allocazione matrice
    int **matrix = (int **)calloc(rows, sizeof(int *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)calloc(cols, sizeof(int));
    }

    // Riempimento matrice con valori casuali
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = 1 + rand() % 10;
        }
    }

    // Stampa della matrice
    printf("\nMatrice generata:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }

    int np;
    printf("\nInserisci il numero di thread: ");
    scanf("%d", &np);

    // Allocazione matrice per i risultati parziali
    int **results = (int **)calloc(np, sizeof(int *));
    for (int i = 0; i < np; i++) {
        results[i] = (int *)calloc(cols, sizeof(int));
        for (int j = 0; j < cols; j++) {
            results[i][j] = 1; // Inizializzo con 1 per il prodotto puntuale
        }
    }

    double tiTime = omp_get_wtime();

    // Calcolo parallelo del prodotto puntuale
    #pragma omp parallel num_threads(np)
    {
        int id = omp_get_thread_num();
        int rowsPerThread = rows / np;
        int rest = rows % np;
        int ti, tf;

        if (id < rest) {
            ti = id * (rowsPerThread + 1);
            tf = ti + (rowsPerThread + 1);
        } else {
            ti = id * rowsPerThread + rest;
            tf = ti + rowsPerThread;
        }

        if (tf > rows) {
            tf = rows;
        }

        for (int i = ti; i < tf; i++) {
            for (int j = 0; j < cols; j++) {
                results[id][j] *= matrix[i][j];
            }
        }

        #pragma omp critical
        {
            printf("\nThread %d – Prodotto puntuale delle righe da %d a %d: \t", id, ti, tf - 1);
            for (int j = 0; j < cols; j++) {
                printf("%d\t", results[id][j]);
            }
            printf("\n");
        }
    }

    double tfTime = omp_get_wtime();
    double executionTime = tfTime - tiTime;

    printf("\nTempo di esecuzione: %f secondi\n", executionTime);

    // Deallocazione matrice
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);

    for (int i = 0; i < np; i++) {
        free(results[i]);
    }
    free(results);

    return 0;
}
