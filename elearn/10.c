/*Implementare un programma parallelo per
l’ambiente multicore con np unità processanti che
impieghi la libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve leggere
una matrice di dimensione N×M, quindi i core devono
collaborare per sommare in parallelo i vettori
corrispondenti alle righe dispari della matrice.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main(){

    int n, m, i, j, np;
    float **a, sum = 0.0;
    double ti, tf;

    srand(time(NULL));

    printf("Inserici il numero di core: \n");
    scanf("%d", &np);

    printf("Inserici il numero di righe: \n");
    scanf("%d", &n);

    printf("Inserici il numero di colonne: \n");
    scanf("%d", &m);

    #pragma omp master
    {
        a = (float **)calloc(n, sizeof(float *));
        for(i = 0; i < n; i++){
            a[i] = (float *)calloc(m, sizeof(float));
        }

        for(i = 0; i < n; i++){
            for(j = 0; j < m; j++){
                a[i][j] = rand()%10;
            }
        }

        printf("Questa e' la matrice A: \n");
        for(i = 0; i < n; i++){
            for(j = 0; j < m; j++){
               printf("A[%d][%d] = %.2f\t", i, j, a[i][j]);
            }
            printf("\n");
        }
    }

    ti = omp_get_wtime();

    #pragma omp parallel for private(i, j) shared(n, m, a) reduction(+:sum) num_threads(np)
    for(i = 0; i < n; i+=2){
        for(j = 0; j < m; j++){
            sum += a[i][j];
        }
    }

    tf = omp_get_wtime();

    printf("La somma dell righe dispari e': %.2f\n", sum);
    printf("Il tempo di esecuzione: %f secondi\n", tf-ti);

    for(i = 0; i < n; i++){
        free(a[i]);
    }
    free(a);

    return 0;
}
