/* Implementazione di un algoritmo parallelo (np core)
per il calcolo dell’operazione c=a+b con a,b vettori di
dimensione N, in ambiente openMP
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main(){

    int np, i, n;
    float *a, *b, c;
    double ti, tf;

    srand(time(NULL));

    printf("Inserisci il numero di core: \n");
    scanf("%d", &np);

    printf("Inserisci la grandezza N: \n");
    scanf("%d", &n);

    #pragma omp master
    {
        // Allocazione dinamica del vettore A
        a = (float *)calloc(n, sizeof(float));

        // Allocazione dinamica del vettore B
        b = (float *)calloc(n, sizeof(float));

        // Riempimento del vettore A e B
        for(i = 0; i < n; i++){
            a[i] = rand()%10;
            b[i] = rand()%10;
        }

        /*
        printf("Questo e' il vettore A: \n");
        for(i = 0; i < n; i++){
            printf("a[%d] = %.2f\t", i, a[i]);
        }

        printf("\n");

        printf("Questo e' il vettore B: \n");
        for(i = 0; i < n; i++){
            printf("b[%d] = %.2f\t", i, b[i]);
        }
        */
    }

    ti = omp_get_wtime();

    #pragma omp parallel for private(i) shared(n) num_threads(np)
    for(i = 0; i < n; i++){
        c += a[i] + b[i];
    }

    printf("\n");

    tf = omp_get_wtime();

    printf("Il tempo impiegato e': %f\n", tf-ti);

    printf("\n");

    printf("Il valore di c e': %f\n", c);

    free(a);
    free(c);

    return 0;
}
