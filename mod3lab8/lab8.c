/* Disciplina: Computacao Concorrente */
/* Autor: Pedro Henrique da Silva Bernardino */
/* Descricao: implementa  o problema dos leitores/escritores priorizando escritores através de semáforos
*/
#include<pthread.h>
#include <semaphore.h>
#include<stdio.h>
#include<stdlib.h>

#define L 4 //numero de threads leitoras
#define E 2 //numero de threads escritoras
#define TAM 10 //tamanho do vetor de inteiros


int l=0,e=0; // número de threads atualmente lendo e escrevendo
sem_t leit, escr; // semáforo de exclusao mútua e de condição para leitores e escritores
pthread_mutex_t em_e, em_l;
int *vetor; //vetor das variáveis compartilhadas



//thread leitora
void * leitor (void * arg) {
  int *id = (int *) arg;
  while(1) {
    //inicio da leitura
    sem_wait(&leit);
    pthread_mutex_lock(&em_l);
    printf("L[%d] quer ler\n", *id);l++; 
    if(l==1) sem_wait(&escr); 
    pthread_mutex_unlock(&em_l); 
    sem_post(&leit);

    //leitura
    printf("Leitora %d esta lendo\n", *id);
    double media = 0;
    for(int i=0;i<TAM;i++)
    {
      printf("%d ", *(vetor+i));
      media += *(vetor+i);
    }
    media /= TAM;
    printf("\n media: %.2lf\n", media);

    //término da leitura
    pthread_mutex_lock(&em_l); 
    printf("L[%d] terminou de ler\n", *id); l--; 
    if(l==0) sem_post(&escr); 
    pthread_mutex_unlock(&em_l); 
  } 

  pthread_exit(NULL);
}



//thread escritora
void * escritor (void * arg) {
  int *id = (int *) arg;
  while(1) {
    //ínicio da escrita
    pthread_mutex_lock(&em_e);
    printf("E[%d] quer escrever\n", *id); e++;
    if(e==1) sem_wait(&leit);
    pthread_mutex_unlock(&em_e);
    sem_wait(&escr);
    
    //escrita
    printf("Escritora %d esta escrevendo\n", *id);
    for (int i = 0; i < TAM; ++i)
    {
      if(i == 0 || i == TAM-1)
        *(vetor+i) = *id; 
      else *(vetor+i) = 2 * *id;
    }

    //fim da escrita
    sem_post(&escr);
    pthread_mutex_lock(&em_e); 
    printf("E[%d] terminou de escrever\n", *id);  e--;
    if(e==0) sem_post(&leit);
    pthread_mutex_unlock(&em_e);
  }
  pthread_exit(NULL);
}




//funcao principal
int main(void) {
  //identificadores das threads
  pthread_t tid[L+E];
  int id[L+E];

  //inicializa os semaforos
  pthread_mutex_init(&em_e, NULL);
  pthread_mutex_init(&em_l, NULL);
  sem_init(&leit, 0, 1);
  sem_init(&escr,0, 1);

  //inicializa o vetor de inteiros
  vetor = (int *) calloc (TAM, sizeof(int));

  //cria as threads leitoras
  for(int i=0; i<L; i++) {
    id[i] = i+1;
    if(pthread_create(&tid[i], NULL, leitor, (void *) &id[i])) exit(-1);
  } 
  
  //cria as threads escritoras
  for(int i=0; i<E; i++) {
    id[i+L] = i+1;
    if(pthread_create(&tid[i+L], NULL, escritor, (void *) &id[i+L])) exit(-1);
  } 

  pthread_exit(NULL);
  return 0;
}