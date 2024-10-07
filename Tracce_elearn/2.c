/*Implementazione di un algoritmo parallelo (np core)
per il calcolo dell’operazione c=alpha*a+b, con a,b
vettori di dimensione N e alpha uno scalare, in
ambiente openMP*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main() {

    int n, i;
    float *a, *b;
    float alpha, c;
    double tempo_inizio, tempo_fine;

    printf("Inserisci la dimensione n dei vettori a e b: \n");
    scanf("%d", &n);

    printf("Inserisci il valore dello scalare alpha: \n");
    scanf("%f", &alpha);

    // Allochiamo i vettori 
    a = (float *)calloc(n, sizeof(float));
    b = (float *)calloc(n, sizeof(float));

    srand(time(NULL));

    // Generiamo casualmente dei valori nei vettori
    for(i = 0; i < n; i++) {
        a[i] = rand() % 10;
        b[i] = rand() % 10;
    }

    printf("Il vettore a: \n");
    for(i = 0; i < n; i++) {
        printf("a[%d] = %f\n", i, a[i]);
    }

    printf("Il vettore b: \n");
    for(i = 0; i < n; i++) {
        printf("b[%d] = %f\n", i, b[i]);
    }

    tempo_inizio = omp_get_wtime();
    #pragma omp parallel for private(i) shared(a, b, n, alpha) reduction(+: c)
    for(i = 0; i < n; i++) {
        c += alpha *(a[i] + b[i]);
    }

    tempo_fine = omp_get_wtime();
    double temp_tot = tempo_fine - tempo_inizio;

    printf("Il risultato e': %f\n", c);

    printf("il tempo effettivo e': %f\n", temp_tot);

    free(a);
    free(b);

    return 0;

}