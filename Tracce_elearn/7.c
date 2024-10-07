/*Implementare un programma parallelo per l’ambiente
multicore con np unità processanti che impieghi la
libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve generare
una matrice A di dimensione N×M. Quindi, i core
devono collaborare per calcolare il minimo tra gli
elementi delle colonne di propria competenza e
conservare tale valore in un vettore c di dimensione M.*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(){

    int i, j, N, M;
    float **A;
    float *c; // Vettore dove si conserva il minimo
    double temp_i, temp_f;

    srand(time(NULL));

    printf("Inserisci la dimensione N: \n");
    scanf("%d", &N);
    printf("Inserisci la dimensione M: \n");
    scanf("%d", &M);

    #pragma omp master
    {
        // Allocazione della matrice A
        A = (float **)calloc(N, sizeof(float *));
        for(i = 0; i < N; i++){
            A[i] = (float *)calloc(M, sizeof(float));
        }

        c = (float *)calloc(M, sizeof(float));

        // Riempimento la matrice A
        for(i = 0; i < N; i++){
            for(j = 0; j < M; j++){
                A[i][j] = rand()%10;
            }
        }

        //Stampa della matrice A
        printf("La matrice A e': \n");
        for(i = 0; i < N; i++){
            for(j = 0; j < M; j++){
                printf("%.1f\t", A[i][j]);
            }
            printf("\n");
        }

    }

    temp_i = omp_get_wtime();
    #pragma omp parallel for shared(A) private(i, j)
    for(j = 0; j < M; j++){
        float min = A[0][j];
        for(i = 0; i < N; i++){
            if(A[i][j] < min){
                min = A[i][j];
            }
        }
        printf("Sono il core: %d e mi occupo della colonna %d\n", omp_get_thread_num(), j);
        c[j] = min;
    }

    temp_f = omp_get_wtime();

    printf("Il vettore min e': \n");
    for(i = 0; i < M; i++){
        printf("c[%d] = %f \n", i, c[i]);
    }

    double tempo = temp_f - temp_i;
    printf("Il tempo e': %f\n", &tempo);

    // Dealloca matrice A e B
    for(i = 0; i < N; i++){
        free(A[i]);
    }

        free(A);
        free(c);

    return 0;
}
