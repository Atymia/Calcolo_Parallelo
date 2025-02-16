/*Il core master deve generare una matrice A di dimensione N * M
e un vettore b di dimensione M;
I core devono collaborare per costruire un vettore c le cui M
componenti assumono il valore del massimo di ogni colonna della
matrice A sommato all'identificativo del thread;
I core collaborano per calcolare in parallelo il prodotto scalare
prod = c * b;
Il core master deve stampare il risultato finale ed il tempo
di esecuzione*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main() {
    int i, j, n, m, np, id;
    float **a, *b, *c, max, prod = 0.0;
    double ti, tf;

    srand(time(NULL));

    printf("Inserire il numero di core \n");
    scanf("%d", &np);

    printf("Inserire il numero di righe(N) della matrice A \n");
    scanf("%d", &n);

    printf("Inserire il numero di colonne(M) della matrice A \n");
    scanf("%d", &m);

    #pragma omp master
    {
        a = (float **)calloc(n, sizeof(float *));
        for(i = 0; i < n; i++) {
            a[i] = (float *)calloc(m, sizeof(float));
        }

        b = (float *)calloc(m, sizeof(float));
        c = (float *)calloc(m, sizeof(float));

        for(i = 0; i < n; i++) {
            for(j = 0; j < m; j++) {
                a[i][j] = rand() % 10;
            }
        }

        for(i = 0; i < m; i++) {
            b[i] = rand() % 10;
        }

        printf("Questa e' la matrice A generata: \n");
        for(i = 0; i < n; i++) {
            for(j = 0; j < m; j++) {
                printf("A[%d][%d] = %.2f\t", i, j, a[i][j]);
            }
            printf("\n");
        }

        printf("\nQuesto e' il vettore b generato: \n");
        for(i = 0; i < m; i++) {
            printf("b[%d] = %.2f\t", i, b[i]);
        }
        printf("\n");
    }

    ti = omp_get_wtime();

    #pragma omp parallel for num_threads(np) private(i, j, max, id) shared(n, m, a, c)
    for(j = 0; j < m; j++) {
        max = a[0][j];
        id = omp_get_thread_num();
        for(i = 0; i < n; i++) {
            if(a[i][j] > max) {
                max = a[i][j];
            }
        }
        c[j] = max + id;
    }

    #pragma omp parallel for num_threads(np) private(i) shared(m, b, c) reduction(+: prod)
    for(j = 0; j < m; j++) {
        prod += b[j] * c[j];
    }

    tf = omp_get_wtime();

    printf("\nQuesto e' il vettore c generato: \n");
    for(i = 0; i < m; i++) {
        printf("c[%d] = %.2f\t", i, c[i]);
    }
    printf("\n");

    printf("Il prodotto scalare e': %.2f\n", prod);
    printf("\nIl tempo di esecuzione e': %.6lf secondi\n", tf - ti);

    for(i = 0; i < n; i++) {
        free(a[i]);
    }
    free(a);
    free(b);
    free(c);

    return 0;
}

