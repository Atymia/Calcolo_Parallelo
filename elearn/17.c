/* Implementare un programma parallelo per
l’ambiente multicore con np unità processanti che
impieghi la libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve leggere
quattro valori scalari a0, a1, a2, a3, quindi i core devono
collaborare per costruire un vettore di dimensione 4np
i cui elementi sono dati dalla somma di tali valori con il
proprio identificativo.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main(){

    int np, n, i;
    float a0, a1, a2, a3, *a;
    double ti, tf;

    srand(time(NULL));

    #pragma omp master
    {
        printf("Inserire il numero di core: \n");
        scanf("%d", &np);

        printf("Inserire il valore dello scalare a0: \n");
        scanf("%f", &a0);

        printf("Inserire il valore dello scalare a1: \n");
        scanf("%f", &a1);

        printf("Inserire il valore dello scalare a2: \n");
        scanf("%f", &a2);

        printf("Inserire il valore dello scalare a3: \n");
        scanf("%f", &a3);

        n = 4*np;

        a = (float *)calloc(n, sizeof(float ));
    }

    ti = omp_get_wtime();

    #pragma omp parallel for num_threads(np) private(i) shared(n, a, a0, a1, a2, a3)
    for(i = 0; i < n; i++){
        int id = i/4;
        if(i%4 == 0){
            a[i] = a0 + id;
            printf("\nIl core %d ha calcolato a[%d] = %.2f\n", omp_get_thread_num(), i, a[i], id);
        }else if(i%4 == 1){
            a[i] = a1 + id;
            printf("Il core %d ha calcolato a[%d] = %.2f\n", omp_get_thread_num(), i, a[i], id);
        }else if(i%4 == 2){
            a[i] = a2 + id;
            printf("Il core %d ha calcolato a[%d] = %.2f\n", omp_get_thread_num(), i, a[i], id);
        }else if(i%4 == 3){
            a[i] = a3 + id;
            printf("Il core %d ha calcolato a[%d] = %.2f\n", omp_get_thread_num(), i, a[i], id);
        }
    }

    tf = omp_get_wtime();

    printf("Il vettore generato: \n");
    for(i = 0; i < n; i++){
        printf("a[%d] = %.2f\n", i, a[i]);
    }

    printf("\nIl tempo di esecuzione e': %f secondi\n", tf-ti);

    free(a);

    return 0;
}
