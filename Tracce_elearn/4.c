/*Implementare un programma parallelo per l’ambiente
multicore con np unità processanti impieghi la libreria
OpenMP. Il programma deve essere organizzato come
segue: il core master deve leggere una matrice di
dimensione N×N, quindi i core devo collaborare per
ricopiare in parallelo gli elementi della diagonale
principale in un vettore di lunghezza N. Infine, i core
devono effettuare la somma degli elementi di tale
vettore in parallelo.*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(){

   int i, j, n;
   float sum = 0;
   float temp_i, temp_f;

   srand(time(NULL));

   printf("Inserisci la dimensione n della matrice: ");
   scanf("%d", &n);

   // Allocazione della matrice
   float **A = (float **)calloc(n, sizeof(float*));
   for(i = 0;i < n; i++) {
       A[i] = (float *)calloc(n, sizeof(float));
   }

   // Riempimento della matrice
   for(i = 0; i < n; i++){
       for(j = 0; j< n; j++) {
           A[i][j] = rand() % 10;
       }
   }

   // Stampa la matrice A
   for(i = 0; i < n; i++){
       for(j = 0; j< n; j++) {
           printf("%.1f\t", A[i][j]);
       }
       printf("\n");
   }

   // Allocazione del vettore c che conterra' gli elementi della diagonale principale
   float *c = (float *)calloc(n, sizeof(float));

   temp_i = omp_get_wtime();

   #pragma omp parallel for shared(A, c) private(i)
   for(i = 0; i < n; i++) {
      c[i] = A[i][i];
   }

   #pragma omp parallel for reduction(+:sum)
   for(i = 0; i < n; i++){
       sum += c[i];
   }

   temp_f = omp_get_wtime();
   float tempo = temp_f - temp_i;

   printf("La somma degli elementi del vettore c e': %.1f\n", sum);

   printf("Il tempo e': %f", tempo);

   // Deallocazione
   for(i = 0; i < n; i++){
       free(A[i]); }

   free(A);
   free(c);
}
