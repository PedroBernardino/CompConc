/* Disciplina: Computacao Concorrente */
/* Aluno: Pedro Henrique */
/* Módulo 1- Trabalho de Implementação */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "timer.h"



int posicaoGlobal = 0, tam; //variavel compartilhada entre as threads
pthread_mutex_t mutex; //variavel de lock para exclusao mutua
int *inicial, *novaPosicao, *resultado;

//funcao que encontra a nova posição dos elementos do vetor
void *EncontraPosicao () {
	int posicaoLocal, contaMenores;
 	do {
    	//--entrada na Seção Crítica
		pthread_mutex_lock(&mutex);
		//--Seção Crítica
     	posicaoLocal = posicaoGlobal; posicaoGlobal++;
    	//--saida da Seção Crítica
     	pthread_mutex_unlock(&mutex);

     	//conta quantos números no vetor são menores do que o número atual.
     	contaMenores = 0;
     	for(int i= 0; i< tam; i++)
     		if(inicial[i]<inicial[posicaoLocal])
     			contaMenores++;
     	novaPosicao[posicaoLocal] = contaMenores;
  	}while(posicaoLocal<tam);

	pthread_exit(NULL);
}
//funcao que ordena o vetor dadas as novas posicoes dos termos.
void *OrdenaVetor () 
{
	int posicaoLocal;

	//--entrada na Seção Crítica
	pthread_mutex_lock(&mutex);
	//--Seção Crítica
     posicaoLocal = posicaoGlobal; posicaoGlobal++;
    //--saida da Seção Crítica
     pthread_mutex_unlock(&mutex);
	while(posicaoLocal<tam){

     	resultado[novaPosicao[posicaoLocal]] = inicial[posicaoLocal];

     	//--entrada na Seção Crítica
		pthread_mutex_lock(&mutex);
		//--Seção Crítica
     	posicaoLocal = posicaoGlobal; posicaoGlobal++;
    	//--saida da Seção Crítica
     	pthread_mutex_unlock(&mutex);
  	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[]) 
{
	int nthreads;
	pthread_t *tid;
 	double inicio, fim;


	//--inicilaiza as variáveis.
	pthread_mutex_init(&mutex, NULL);


	if (argc<3)	
	{
		printf("Digite o tamanho do vetor e número de threads ao executar o arquivo.\n");
		return 1;
	}

	tam = atoi(argv[1]);
	nthreads = atoi(argv[2]);

	tid = (pthread_t*) malloc (sizeof(pthread_t) * nthreads);
	if (tid == NULL) {
      printf("--ERRO: malloc()\n"); 
      return -1;
    }

    inicial = (int*) malloc (sizeof(int) * tam);
	if (inicial == NULL) {
      printf("--ERRO: malloc()\n"); 
      return -1;
    }

    novaPosicao = (int*) malloc (sizeof(int) * tam);
	if (novaPosicao == NULL) {
      printf("--ERRO: malloc()\n"); 
      return -1;
    }

    resultado = (int*) malloc (sizeof(int) * tam);
	if (resultado == NULL) {
      printf("--ERRO: malloc()\n"); 
      return -1;
    }


    //inicializa o vetor
	for (int i = 0; i < tam; ++i)
	{
		inicial[i] = tam-i;
	}


	GET_TIME(inicio);

	//criacao das threads para encontrar a posicao dos vetores
	for (int thread = 0; thread < nthreads; ++thread)
	{
		if (pthread_create(tid+thread, NULL, EncontraPosicao, NULL))
		{
      		printf("--ERRO: pthread_create()\n"); 
      		return -2;
    	}
	}

	//--espera todas as threads terminarem
	for (int thread=0; thread<nthreads; thread++) 
	{
    	if (pthread_join(*(tid+thread), NULL))
    	{
        	printf("--ERRO: pthread_join() \n");
        	return -3; 
        }
    } 	

    posicaoGlobal = 0;

    //criacao das threads para posicionar os elementos no vetor ordenado
	for (int thread = 0; thread < nthreads; ++thread)
	{
		if (pthread_create(tid+thread, NULL, OrdenaVetor, NULL))
		{
      		printf("--ERRO: pthread_create()\n"); 
      		return -2;
    	}
	}

	//--espera todas as threads terminarem
	for (int thread=0; thread<nthreads; thread++) 
	{
    	if (pthread_join(*(tid+thread), NULL)) 
    	{
        	printf("--ERRO: pthread_join() \n");
        	return -3; 
        }
    } 

    GET_TIME(fim);

/*    for (int i = 0; i < tam; ++i)
    {
    	printf("%d ", resultado[i]);
    }*/

    printf("Tempo de execução: %.3lf\n", fim-inicio);
    return 0;
}