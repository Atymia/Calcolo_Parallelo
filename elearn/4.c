/*
 Implementare un programma parallelo per l’ambiente
multicore con np unità processanti impieghi la libreria
OpenMP. Il programma deve essere organizzato come
segue: il core master deve leggere una matrice di
dimensione N×N, quindi i core devo collaborare per
ricopiare in parallelo gli elementi della diagonale
principale in un vettore di lunghezza N. Infine, i core
devono effettuare la somma degli elementi di tale
vettore in parallelo.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main(){

    int i, j, n, np;
    float **a, *b, somma;
    double ti, tf;

    srand(time(NULL));

    printf("Inserisci il numero di core: \n");
    scanf("%d", &np);

    printf("Inserisci il numero di righe(N) e di colonne(N) della matrice A: \n");
    scanf("%d", &n);

    #pragma omp master
    {
        a = (float **)calloc(n, sizeof(float *));
        for(i = 0; i < n; i++){
            a[i] = (float *)calloc(n, sizeof(float));
        }

        b = (float *)calloc(n, sizeof(float));

        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                a[i][j] = rand()%10;
            }
        }

        printf("La matrice generata A e': \n");
        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                printf("a[%d][%d] = %.2f\t", i, j, a[i][j]);
            }
            printf("\n");
        }
    }

    ti = omp_get_wtime();

    #pragma omp parallel for private(i, j) shared(a, b, somma) num_threads(np)
    for(i = 0; i < n; i++){
        b[i] = a[i][i];
        somma += b[i];
    }

    tf = omp_get_wtime();

    printf("Il tmepo impiegato e': %f\n", tf-ti);

    printf("La somma e': %.2f\n", somma);

    return 0;
}
