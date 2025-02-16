/*Implementare un programma parallelo per l'ambiente multicore con np unita' processanti che impieghi la liberria OpenMP.

1)il core master deve generare una matrice A di NXM
2)ogni core deve estrarre un blocco riga dalla matrice generata e conservarlo in una sottomatrice locale A_LOC_I, i=0,...,np-1;
3)i core devono collabborare per effettuare, in parallelo, il prodotto puntuale tra le sottomatrici locali A_loc_i ottenute.
4)infine il core master deve stampare il risultato finale ed tempo di esecuzione
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(){
    int i, j, k, n, m, np;
    float **a;
    unsigned long *resVector, res = 0;
    double ti, tf;

    srand(time(NULL));

    printf("Inserire il numero di core: \n");
    scanf("%d", &np);

    printf("Inserire il numero di righe(N) della matrice A: \n");
    scanf("%d", &n);

    printf("Inserire il numero di colonne(M) della matrice A: \n");
    scanf("%d", &m);

    #pragma omp master
    {
        a = (float **)calloc(n, sizeof(float *));

        for(i = 0; i < n; i++){
            a[i] = (float *)calloc(m, sizeof(float));
        }

        for(i = 0; i < n; i++){
            for(j = 0; j < m; j++){
                a[i][j] = (rand()%10)+1;
            }
        }

        printf("\nMatrice A:\n");
        for(i = 0; i < n; i++){
            for(j = 0; j < m; j++){
                printf("A[%d][%d] = %.2f\t", i, j, a[i][j]);
            }
            printf("\n");
        }
    }

    ti = omp_get_wtime();

    #pragma omp parallel num_threads(np) shared(a, n, m) reduction(*:resVector)
    {
        int nloc = n/np;
        int remainder = n%np;
        int id = omp_get_thread_num();
        int step = 0;

        unsigned long *a_loc_i;
        a_loc_i = (float *)calloc(np, sizeof(float));

        if(id < remainder){
            nloc++;
        }else{
            step = remainder;
        }

        for(size_t i = 0; i < nloc; i++){
            for(size_t j = 0; j < m; j++){
                a_loc_i[j] = a[i+nloc*id+step][j];
                resVector[j] *= a_loc_i[j];
            }
            #pragma omp critical
            {
                printf("\nCore: %d\n", id);
                for(i = 0; i < m; i++){
                    printf("a_loc_i[%d] = %.2f\t", i, a_loc_i[i]);
                }
                printf("\n");
            }
        }
        free(a_loc_i);
    }

    #pragma omp parallel for num_threads(np) reduction(+:res)
    for(size_t i = 0; i < m; i++){
        res += resVector[i];
    }

    tf = omp_get_wtime();

    #pragma omp master
    {
        printf("\nVettore prodotti: \n");
        for(i = 0; i < n; i++){
            printf("resVector[%d] = %.2f\t", i, resVector[i]);
        }

        printf("\nSomma Prodotti: %lu\n", res);
        printf("Il tempo di esecuzione: %f secondi\n", tf-ti);
    }

    for(i = 0; i < n; i++){
        free(a[i]);
    }
    free(a);
    free(resVector);

    return 0;
}
