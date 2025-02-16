/* Il core master deve generare due vettori a e b di dimensione N;
I core devono collaborare per calcolare il prodotto scalare alpha tra i vettori a e b;
Ogni core deve utilizzare il valore di alpha per costruire un vettore c,
dove ogni componente c[i] è calcolata come (alpha + np) * b[i];
Il core master deve stampare il risultato finale del vettore c e il valore di alpha, insieme al tempo totale di esecuzione. */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(){

    int i, np, n;
    float *a, *b, *c, alpha = 0.0, prod;
    double ti, tf;

    srand(time(NULL));

    printf("Inserire il numero di core: \n");
    scanf("%d", &np);

    printf("Inserire la grandezza N: \n");
    scanf("%d", &n);

    printf("Inserire alpha: \n");
    scanf("%f", &alpha);

    #pragma omp master
    {
        a = (float *)calloc(n, sizeof(float));
        b = (float *)calloc(n, sizeof(float));
        c = (float *)calloc(n, sizeof(float));

        for(i = 0; i < n; i++){
            a[i] = rand()%10;
            b[i] = rand()%10;
        }

        printf("\nIl vettore generato a: \n");
        for(i = 0; i < n; i++){
            printf("a[%d] = %.2f\t", i, a[i]);
        }

        printf("\nIl vettore generato b: \n");
        for(i = 0; i < n; i++){
            printf("b[%d] = %.2f\t", i, b[i]);
        }
    }

    ti = omp_get_wtime();
    #pragma omp parallel for num_threads(np) private(i) shared(a, b) reduction(+:alpha)
    for(i = 0; i < n; i++){
        alpha += a[i]*b[i];
    }

    #pragma omp parallel for num_threads(np) private(i) shared(alpha,c, b)
    for(i = 0; i < n; i++){
        c[i] = (alpha + np)*b[i];
    }

    tf = omp_get_wtime();

    printf("\nIl valore di alpha e': %f\n", alpha);

    printf("\nIl vettore c: \n");
    for(i = 0; i < n; i++){
        printf("c[%d] = %.2f\t", i, c[i]);
    }

    printf("\nIl tempo di esecuzione: %f secondi", tf-ti);

    free(a);
    free(b);
    free(c);

    return 0;
}
