/* Implementare un programma parallelo per
l’ambiente multicore con np unità processanti che
impieghi la libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve leggere un
vettore a, di dimensione N ed uno scalare beta. Quindi
i core devo collaborare per verificare se nel vettore
esista almeno un elemento uguale al valore beta.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main(){

    int i, n, np, t=0;
    float beta, *a;
    double ti, tf;

    srand(time(NULL));

    #pragma omp master
    {
        printf("Inserire il numero di core: \n");
        scanf("%d", &np);

        printf("Inserire la grandezza N: \n");
        scanf("%d", &n);

        printf("Inserire alpha: \n");
        scanf("%f", &beta);
    }

    a = (float *)calloc(n, sizeof(float));

    for(i = 0; i < n; i++){
        a[i] = rand()%10;
    }

    for(i = 0; i < n; i++){
        printf("a[%d] = %.2f\t", i, a[i]);
    }

    ti = omp_get_wtime();

    #pragma omp parallel for private(i) shared(beta, a, n) num_threads(np)
    for(i = 0; i < n; i++){
        if(beta == a[i]){
            t++;
        }
    }

    tf = omp_get_wtime();

    if(t > 0){
        printf("\nbeta e' stato trovato %d volte\n", t);
    }else{
        printf("\nbeta non e' stato trovato\n", tf-ti);
    }

    free(a);

    return 0;
}
