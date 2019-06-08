#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define n_threads 2


int vetor[100000];
int aux[100000];
int length;
pthread_mutex_t trava;
pthread_mutex_t travadao, travadao1;
int start_threads = 1;

//void merge( int comeco, int meio, int fim); 
void merge( int ini, int meio, int fim); 
void mergeSort(int comeco, int fim);
void* merge_thread(void *arg);
int scan();
pthread_t threads[n_threads];


int main(int argc, char **argv){
  int i = 0,j = 0;
  int ind[5];
 	

  	length = scan();
  	//printf("O tamanho do vetor eh: %d\n\n", length);
  	if(length >= 4){
    	start_threads = n_threads;
  	}
  	//printf("start_threads: %d\n", start_threads);

  	for(i = 0; i < start_threads; i++) {
    	ind[i] = i;
    	//printf("Indice i = %d\n", ind[i]);
    	pthread_create(&(threads[i]), NULL, merge_thread, (void *)&(ind[i]));
  	}

  	 for(int d; d < start_threads; d++){
    	pthread_join(threads[d], NULL);
      //printf("encerrando a thread %d\n", d);
  	}
  	
    if(start_threads > 1){
      merge(0,(length/2)-1,length-1);  
    }
    

  	for(i=0;i < length - 1;i++){
    	printf("%d ", vetor[i]);
  	}
    printf("%d\n", vetor[length-1]);
  	return 0;
}

void merge(int ini, int meio, int fim){
    int i=ini,j=meio+1,k=0; //indices da metade inf, sup e aux respc.
    while(i<=meio && j<=fim){
        if(vetor[i] <= vetor[j])
            aux[k++] = vetor[i++];
        else
            aux[k++] = vetor[j++];
        }
    while(i<=meio)
        aux[k++] = vetor[i++];
    while(j<=fim)
        aux[k++] = vetor[j++];
    for(i=ini, k=0 ; i<= fim; i++, k++) //copia aux para v
        vetor[i]=aux[k];
}

void mergeSort(int comeco, int fim){
	//printf("Entrei no mergeSort\n");
    
   	if (comeco < fim) {
      int meio = (fim+comeco)/2;

      //printf("Inicio: %d, meio: %d, fim  %d\n", comeco, meio, fim); 

      mergeSort(comeco, meio);
      mergeSort(meio+1, fim);
      merge(comeco, meio, fim);
      //printf("saindo do mergeSort \n");
      return;
    }
}

void *merge_thread(void *arg){
    int size;
    int inicio, meio, fim;
    int* N = (int*)arg;
    int parte = (*N);
    size = length;

    //printf("Entrei na thread de numero: %d\n", parte);




    pthread_mutex_lock(&trava);
    inicio = parte * (size / start_threads); 
    fim = (((parte + 1) * (size / start_threads)) - 1);
    //printf("Inicio: %d, meio: %d, fim  %d\n", inicio, meio, fim); 
    meio = (fim + inicio) / 2;
    //printf("Na thread %d o valor de inicio eh %d, o valor de meio eh: %d e o valor de fim eh: %d \n", parte, inicio, meio, fim);    
    //parte++; 
    pthread_mutex_unlock(&trava); 


    
    //printf("Entrei na thread de numero: %d\n", parte);
    if (inicio < fim) {
      //printf("Inicio: %d, meio: %d, fim  %d\n", inicio, meio, fim);
      pthread_mutex_lock(&travadao);
      mergeSort(inicio, meio);
      pthread_mutex_unlock(&travadao);
      //printf("vai passar\n");
      pthread_mutex_lock(&travadao1); 
      mergeSort(meio + 1, fim);
      pthread_mutex_unlock(&travadao1);
      //printf("passou o final\n"); 
      merge(inicio, meio, fim);
    }
     

  	return NULL;
}

int scan(){
 	char space;
  	int size = 0;
  	while(space != '\n'){
    	scanf("%d", &vetor[size]);
    	size++;
    	space = getchar();
  	}
  	return size;
}
