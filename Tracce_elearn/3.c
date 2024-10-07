/*Implementazione di un algoritmo parallelo (np core)
per il calcolo degli elementi di un vettore c, i cui valori
sono ottenuti moltiplicando l’identificativo del core per
gli elementi di un vettore a di dimensione N, in
ambiente openMP*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main() {

    int n, i;
    int np;
    double tempo_inizio, tempo_fine;

    printf("Numero core: ");
    scanf("%d", &np);

    printf("Inserisci la dimensione n del vettore a: ");
    scanf("%d", &n);

    // Allocazione vettore a
    float *a = (float *)calloc(n, sizeof(float));

    for(i = 0; i < n; i++) {
        a[i] = rand() % 100;
    }

    printf("Il vettore a: \n");
    for(i = 0; i < n; i++) {
        printf("a[%d] = %f\n", i, a[i]);
    }

    float *c = (float *)calloc(n, sizeof(float));

    tempo_inizio = omp_get_wtime();
    #pragma omp parallel for private(i) shared(a, n) num_threads(np)
    for(i = 0; i < n; i++) {
        int thread_id = omp_get_thread_num();
        c[i] = thread_id * a[i];
    }

    tempo_fine = omp_get_wtime();
    double temp_tot = tempo_fine - tempo_inizio;

    // Stampo il vettore c
    printf("Il vettore c: \n");
    for(i = 0; i < n; i++) {
        printf("c[%d] = %f\n", i, c[i]);
    }

    printf("il tempo effettivo e': %f\n", temp_tot);

    free(a);
    free(c);

    return 0;
}