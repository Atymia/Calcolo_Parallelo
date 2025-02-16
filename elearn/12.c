/*Implementare un programma parallelo per
l’ambiente multicore con np unità processanti che
impieghi la libreria OpenMP. Il programma deve essere
organizzato come segue: il core master generare una
matrice di dimensione N×M. Quindi, ogni core deve
estrarre la riga della matrice con indice uguale al
proprio identificativo e conservarla in un vettore locale;
successivamente deve sommare gli elementi di tale
vettore e conservare il risultato in un vettore c di
dimensione np. Infine, i core devo collaborare per
individuare in massimo tra gli elementi del vettore c.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main() {
    int i, j, n, m, np;
    float **a, *c, max = 0.0;
    double ti, tf;

    srand(time(NULL));

    printf("Inserisci il numero di core: \n");
    scanf("%d", &np);

    printf("Inserisci il numero di righe: \n");
    scanf("%d", &n);

    printf("Inserisci il numero di colonne: \n");
    scanf("%d", &m);

    #pragma omp master
    {
        a = (float **)calloc(n, sizeof(float *));
        for (i = 0; i < n; i++) {
            a[i] = (float *)calloc(m, sizeof(float));
        }

        c = (float *)calloc(np, sizeof(float));

        for (i = 0; i < n; i++) {
            for (j = 0; j < m; j++) {
                a[i][j] = rand()%10;
            }
        }

        printf("Questa e' la matrice generata A:\n");
        for (i = 0; i < n; i++) {
            for (j = 0; j < m; j++) {
                printf("A[%d][%d] = %.2f\t", i, j, a[i][j]);
            }
            printf("\n");
        }

        printf("\n");
    }

    ti = omp_get_wtime();

    // Calcolo parallelo delle somme e del massimo
    #pragma omp parallel for private(j) shared(a, c) reduction(max: max) num_threads(np)
    for (i = 0; i < np; i++) {
        float sum = 0.0;
        printf("Thread %d sta lavorando sulla riga %d \n", omp_get_thread_num(), i);
        for (j = 0; j < m; j++) {
            sum += a[i][j];
        }
        c[i] = sum;
        if (sum > max) {
            max = sum;
        }
    }

    tf = omp_get_wtime();

    printf("\n");

    printf("Questo e' il vettore delle somme c:\n");
    for (i = 0; i < np; i++) {
        printf("c[%d] = %.2f\n", i, c[i]);
    }

    printf("\n");

    printf("Il massimo elemento di c e': %.2f\n", max);
    printf("Questo e' il tempo di esecuzione: %f secondi\n", tf - ti);

    for (i = 0; i < n; i++) {
        free(a[i]);
    }
    free(a);
    free(c);

    return 0;
}

