/* Disciplina: Computacao Concorrente */
/* Autor: Pedro Henrique da Silva Bernardino */
/* Descricao: implementa  o problema dos leitores/escritores priorizando escritores através de semáforos
*/

#include<pthread.h>
#include <semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define L 4 //numero de threads leitoras
#define E 2 //numero de threads escritoras
#define TAM 10 //tamanho do vetor de inteiros

//variaveis do problema

int lendo = 0, filaescr = 0; // contador de leitores lendo atualmente e de escritores na fila
sem_t mutex, leitura, escrita; // semáforo de exclusao mútua para seções criticas, e os de condição para leitores e escritores
int *vetor; //vetor das variáveis compartilhadas

void ler(int id) {

	//waits e posts apenas para garantir que os prints serão operações atômicas
	sem_wait(&mutex);
	printf("L[%d] quer ler\n", id);
	sem_post(&mutex);


	sem_wait(&leitura);


	sem_wait(&mutex);
	printf("L[%d] passou do semáforo\n", id);
	sem_post(&mutex);

	// faz a leitura
	double media = 0;
	lendo++;
    printf("Leitora %d esta lendo\n", id);
    for(int i=0;i<TAM;i++)
    {
      printf("%d ", *(vetor+i));
      media += *(vetor+i);
    }
    media /= TAM;
    printf("\n media: %.2lf\n", media);

    //da vaga a outra thread leitora apenas se nenhum escritor quiser escrever
	sem_wait(&mutex);
	lendo--;
	printf("L[%d] terminou de ler\n", id);
	if(lendo == 0)
		sem_post(&escrita);
	if(filaescr == 0)
		sem_post(&leitura);
	sem_post(&mutex);

}

void escrever(int id)
{
	sem_wait(&mutex);
	printf("E[%d] quer escrever\n", id);
	filaescr++;
	sem_post(&mutex);

	sem_wait(&escrita);

	sem_wait(&mutex);
	printf("E[%d] passou do semáforo\n", id);
	filaescr--;
	sem_post(&mutex);


	// faz a escrita

	printf("Escritora %d esta escrevendo\n", id);
    for (int i = 0; i < TAM; ++i)
    {
      if(i == 0 || i == TAM-1)
        *(vetor+i) = id; 
      else *(vetor+i) = 2 * id;
    }

	sem_wait(&mutex);
    printf("E[%d] terminou de escrever\n", id);
	//libera as threads leitoras somente se nenhuma outra thread escritora quiser escrever
	if(filaescr == 0)
		for (int i = 0; i < L; ++i)
			sem_post(&leitura);
	else sem_post(&escrita);
	sem_post(&mutex);


	
}

//thread leitora
void * leitor (void * arg) {
  int *id = (int *) arg;
  while(1)
  	ler(*id);
  free(arg);
  pthread_exit(NULL);
}

//thread escritora
void * escritor (void * arg) {
  int *id = (int *) arg;
  while(1)
  	escrever(*id);
  free(arg);
  pthread_exit(NULL);
}




//funcao principal
int main(void) {
  //identificadores das threads
  pthread_t tid[L+E];
  int id[L+E];

  //inicializa os semaforos
	sem_init(&mutex, 0, 1);
	sem_init(&leitura, 0, L);
	sem_init(&escrita,0, 0);

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