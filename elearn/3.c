/*
Implementazione di un algoritmo parallelo (np core)
per il calcolo degli elementi di un vettore c, i cui valori
sono ottenuti moltiplicando l’identificativo del core per
gli elementi di un vettore a di dimensione N, in
ambiente openMP
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main(){

    int n, i, np, id;
    float *a, *c;
    double ti, tf;

    srand(time(NULL));

    printf("Inserisci il numero di core: \n");
    scanf("%d", &np);

    printf("Inserisci la grandezza N: \n");
    scanf("%d", &n);

    #pragma omp master
    {
        a = (float *)calloc(n, sizeof(float));
        c = (float *)calloc(n, sizeof(float));

        for(i = 0; i < n; i++){
            a[i] = rand()%10;
        }

        for(i = 0; i < n; i++){
            printf("a[%d] = %.2f\t\n", i, a[i]);
        }
    }

    ti = omp_get_wtime();
    #pragma omp parallel for private(i) shared(c, a, n, id) num_threads(np)
    for(i = 0; i < n; i++){
        id = omp_get_thread_num();
        c[i] = id*a[i];
    }

    tf = omp_get_wtime();

    printf("Il tempo impiegato e': %f\n", tf-ti);

    printf("Il vettore risultato C e': \n");
    for(i = 0; i < n; i++){
        printf("c[%d] = %.2f\t\n", i, c[i]);
    }

    free(a);
    free(c);

    return 0;
}
