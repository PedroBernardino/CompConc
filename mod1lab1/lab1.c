/* Disciplina: Computacao Concorrente */
/* Aluno: Pedro Henrique */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: Incrementado todos os elementos de um vetor em 1 usando threads em C com passagem de um argumento */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define TAMVETOR  100 //tamanho do vetor 10 <= TAMVETOR <= 100
#define NTHREADS  2 //total de threads a serem criadas

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int inicioVetor, numThreads, maxVetor;
   int* vetor;
} t_Args;

//funcao executada pelas threads
void *PrintHello (void *arg) {
  t_Args *args = (t_Args *) arg;

  for (int i = args->inicioVetor; i < args->maxVetor; i+=args->numThreads)
  {
  	args->vetor[i]++;
  }

  free(arg); //aqui pode liberar a alocacao feita na main

  pthread_exit(NULL);
}

//funcao principal do programa
int main() {
  pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  int thread; //variavel auxiliar
  int vetor[TAMVETOR]; //cria o vetor de elementos.
  

  //preenche o vetor de elemntos
  for (int i = 0; i < TAMVETOR; ++i)
  {
  	vetor[i] = (i+2)*5-3;
  }

  t_Args *arg; //receberá os argumentos para a thread

  for(thread=0; thread<NTHREADS; thread++) {

    //preparando os argumentos para as threads
    arg = malloc(sizeof(t_Args));
    if (arg == NULL) {
      printf("--ERRO: malloc()\n"); exit(-1);
    }
    arg->vetor = vetor;
    arg->inicioVetor = thread; 
    arg->numThreads = NTHREADS; 
    arg->maxVetor = TAMVETOR;

    //criando as threads
    if (pthread_create(&tid_sistema[thread], NULL, PrintHello, (void*) arg)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  //--espera todas as threads terminarem
  for (thread=0; thread<NTHREADS; thread++) {
    if (pthread_join(tid_sistema[thread], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
  }

  //printa os elementos do vetor após a alteração.
  for (int i = 0; i < TAMVETOR; ++i)
  {
  	printf("Vetor[%d] = %d\n", i+1, vetor[i]);
  }
}
