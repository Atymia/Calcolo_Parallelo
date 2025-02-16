/*
Implementazione di un algoritmo parallelo (np core)
per il calcolo dell’operazione c=alpha*a+b, con a,b
vettori di dimensione N e alpha uno scalare, in
ambiente openMP
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main(){

    int i, n, np;
    float *a, *b, c, alpha;
    double ti, tf;

    srand(time(NULL));

    printf("Inserisci il numero di core: \n");
    scanf("%d", &np);

    printf("Inserisci la grandezza N: \n");
    scanf("%d", &n);

    printf("Iserisci lo scalare alpha: \n");
    scanf("%f", &alpha);

    #pragma omp master
    {
        a = (float *)calloc(n, sizeof(float));

        b = (float *)calloc(n, sizeof(float));

        for(i = 0; i < n; i++){
            a[i] = rand()%10;
            b[i] = rand()%10;
        }

        printf("Questo e' il vettore a: \n");
        for(i = 0; i < n; i++){
            printf("a[%d] = %.1f\t", i, a[i]);
            printf("\n");
        }

        printf("Questo e' il vettore b: \n");
        for(i = 0; i < n; i++){
            printf("b[%d] = %.1f\t", i, b[i]);
            printf("\n");
        }
    }

    ti = omp_get_wtime();

    #pragma omp parallel for private(i) shared(n, a, b) num_threads(np) reduction(+:c)
    for(i = 0; i < n; i++){
        c += alpha*(a[i]+b[i]);
    }

    tf = omp_get_wtime();

    printf("Il tempo impiegato e': %f\n", tf-ti);

    printf("Il risultato e: %.2f\n", c);

    free(a);
    free(b);

    return 0;
}
