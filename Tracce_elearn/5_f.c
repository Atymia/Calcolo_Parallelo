/*Implementare un programma parallelo per l’ambiente
multicore con np unità processanti che impieghi la
libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve generare
una matrice di dimensione N×N, ogni core deve
estrarre N/np righe e calcolare il prodotto puntuale tra
i vettori corrispondenti alle righe estratte.*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(){

int i,j,n,np,nloc;

float **Matr,*a;
a=(float *)calloc(n,sizeof(float));


double ti,tf;
np=omp_get_num_procs();
printf("numero di thread : %d\n",np);

printf("inserire la grandezza della matrice\n");
scanf("%d",&n);
nloc=n/np;
printf("numero di righe : %d\n",nloc);

Matr=(float **)calloc(n,sizeof(float *));
for(i=0;i<n;i++){
for(j=0;j<n;j++){
Matr[i]=(float *)calloc(n,sizeof(float ));
}
}


for(i=0;i<n;i++){
for(j=0;j<n;j++){
Matr[i][j]=rand() % 20;
}
}

printf("la matrice :\n");
for(i=0;i<n;i++){
for(j=0;j<n;j++){
printf("%.2f\t",Matr[i][j]);
}
printf("\n");
}



ti=omp_get_wtime();
#pragma omp parallel for  private(i,j) shared(Matr,n,a)  
for(j=0;j<n;j++){
a[j]=Matr[0][j];
for(i=1;i<nloc;i++){
a[j]=Matr[i][j]*a[j];
}
}

tf=omp_get_wtime();
printf("il valore del tempo dell'algoritmo in parallelo %f\n",tf-ti);
     printf("il vettore risultato:\n");
for(i=0;i<n;i++){
printf("%.2f\t",a[i]);

}
free(a);
// Deallocazione Matrice
   for(i = 0; i < n; i++){
       free(Matr[i]); }
return 0;
     }
     

   


