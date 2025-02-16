/* Il core master deve generare una matrice A di dimensione N x M e uno scalare alpha;
I core devono collaborare per costruire una matrice B, dove ogni elemento B[i][j] è calcolato come (alpha * id) + A[i][j], con 'id' che rappresenta l'identificativo del thread;
I core devono collaborare per trovare il massimo valore all'interno della matrice B;
Il core master deve stampare il valore massimo trovato e il tempo totale di esecuzione;
Alla fine, il core master dealloca la memoria per entrambe le matrici. */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(){

    int n, m, i, j, np;
    float **a, **b, alpha, max;
    double ti, tf;

    srand(time(NULL));

    printf("Inserire il numero di core: \n");
    scanf("%d", &np);

    printf("Inserire il numero di righe della matrice A: \n");
    scanf("%d", &n);

    printf("Inserire il numero di colonne della matrice A: \n");
    scanf("%d", &m);

    printf("Inserire lo scalare alpha: \n");
    scanf("%f", &alpha);

    #pragma omp master
    {
        a = (float **)calloc(n, sizeof(float *));
        b = (float **)calloc(n, sizeof(float *));
        for(i = 0; i < n; i++){
            a[i] = (float *)calloc(m, sizeof(float));
            b[i] = (float *)calloc(m, sizeof(float));
        }

        for(i = 0; i < n; i++){
            for(j = 0; j < m; j++){
                a[i][j] = (rand()%10)+1;
            }
        }

        printf("Questa e' la matrice A: \n");
        for(i = 0; i < n; i++){
            for(j = 0; j < m; j++){
                printf("A[%d][%d] = %.2f\t", i, j, a[i][j]);
            }
            printf("\n");
        }
    }

    ti = omp_get_wtime();

    #pragma omp parallel num_threads(np) private(i, j) shared(a, b, alpha)
    for(i = 0; i < n; i++){
        int id = omp_get_thread_num();
        printf("id: %d\n", id);
        for(j = 0; j < m; j++){
            b[i][j] = (alpha * id) + a[i][j];
        }
    }

    #pragma omp parallel num_threads(np) private(i, j) reduction(max:max)
    for(i = 0; i < n; i++){
        max = b[0][j];
        for(j = 0; j < m; j++){
            if(b[i][j] > max){
            max = b[i][j];
            }
        }
    }

    tf = omp_get_wtime();

    printf("\nQuesta e' la matrice B generata: \n");
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            printf("B[%d][%d] = %.2f\t", i, j, b[i][j]);
        }
        printf("\n");
    }

    printf("\nIl max della matrice B e': %f\n", max);

    printf("\nIl tempo di esecuzione e': %f\n", tf-ti);

    for(i = 0; i < n; i++){
        free(a[i]);
        free(b[i]);
    }
    free(a);
    free(b);

    return 0;
}
