/*Implementazione di un algoritmo parallelo (np core)
per il calcolo dell’operazione c=a+b con a,b vettori di
dimensione N, in ambiente openMP (pensato per c come vettore)*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void riempiVettore(float* vettore, int n);
void stampaVettore(float* vettore, int n);

int main() {

    int n, i;

    printf("Inserisci la dimensione n dei vettori a e b: \n");
    scanf("%d", &n);

    float* a = (float *)calloc(n, sizeof(float));
    float* b = (float *)calloc(n, sizeof(float));
    float* c = (float *)calloc(n, sizeof(float));

    srand(time(NULL));

    riempiVettore(a, n);
    riempiVettore(b, n);


    #pragma omp parallel for private(i) shared(a, b, n)
    for(i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }

    printf("Il vettore c e' uguale a: \n");
    stampaVettore(c, n);

    free(a);
    free(b);
    free(c);

    return 0;

}

void riempiVettore(float* vettore, int n) {
    for(int i = 0; i < n; i++){
        vettore[i] = rand() % 10;
    }
}

void stampaVettore(float* vettore, int n) {
    for(int i = 0; i < n; i++){
        printf("c[%d] = %f\n", i, vettore[i]);
    }
}