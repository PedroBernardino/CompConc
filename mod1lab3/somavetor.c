/* Multiplicador de matrizes quadradas
   Computação Concorrente
   Módulo 1 - Laboratório 2
   Pedro Henrique da Silva Bernardino  */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float *vetor; //vetor do programa
float *result; //matriz de saída
int dim; //dimensão das matrizes quadradas
int nthreads; // número de threads

//funcao executada pelas threads
void *MultiplicaLinhaMatriz (void *linhaInicio) {

  int inicio = *((int *)linhaInicio);//linha da matriz em que a thread vai começar a operar
  for (int i = inicio; i < dim; i+= nthreads)
  	//efetua a multiplicação de uma linha da mat1 pela mat2;
  	for(int j = 0; j < dim; j++)
  		for(int k = 0; k < dim; k++)
  			*(result+(dim*i)+j) += *(mat1+(dim*i)+k) * *(mat2+(dim*k)+j);


  pthread_exit(NULL);
}


int main(int argc, char* argv[])
{
	int *linhas;
	pthread_t *tid;
	double inicio, parcial, fim, elapsed;

	//recebe os argumentos das linhas de comando
	if (argc<3)
	{
		printf("Digite a dimensão das matrizes e o número de threads ao executar o arquivo.\n");
		return 1;
	}
	dim = atoi(argv[1]);
	nthreads = atoi(argv[2]);

	//cria as matrizes usadas no programa
	GET_TIME(inicio);
	parcial = inicio;

	tid = (pthread_t*) malloc (sizeof(pthread_t) * nthreads);
	if (tid == NULL) {
      printf("--ERRO: malloc()\n"); exit(-1);
    }

	mat1 = (float *) malloc (sizeof(float) * dim * dim);
	if (mat1 == NULL) {
      printf("--ERRO: malloc()\n"); exit(-1);
    }

	mat2 = (float *) malloc (sizeof(float) * dim * dim);
	if (mat2 == NULL) {
      printf("--ERRO: malloc()\n"); exit(-1);
    }

	result = (float *) calloc (dim*dim, sizeof(float));
	if (result == NULL) {
      printf("--ERRO: malloc()\n"); exit(-1);
    }

	linhas = (int*) malloc(sizeof(int)*nthreads);
	if (linhas == NULL) {
      printf("--ERRO: malloc()\n"); exit(-1);
    }

	// gera valores para as matrizes de entrada
	for (int i = 0; i < dim; ++i)
	{
		for (int j = 0; j < dim; ++j)
		{
			*(mat1+(dim*i)+j) = 2*i+4*j;
			*(mat2+(dim*i)+j) = 3*j-2*i;
		}
	}

	GET_TIME(fim);
	elapsed = fim - parcial;
    printf("O tempo de inicialização das estruturas de dados foi de %lf segundos. \n", elapsed);
    parcial = fim;

	//cria as threads
	for (int thread = 0; thread < nthreads; ++thread)
	{
		*(linhas+thread) = thread;  
		if (pthread_create(tid+thread, NULL, MultiplicaLinhaMatriz, (void*)(linhas+thread)))
		{
      		printf("--ERRO: pthread_create()\n"); exit(-1);
    	}
	}

	//--espera todas as threads terminarem
	for (int thread=0; thread<nthreads; thread++) 
	{
    	if (pthread_join(*(tid+thread), NULL)) 
    	{
        	printf("--ERRO: pthread_join() \n");
        	exit(-1); 
        }
    } 

    GET_TIME(fim);
	elapsed = fim - parcial;
    printf("O tempo de cálculo da multiplicação pelas threads foi de %lf segundos. \n", elapsed);
    parcial = fim;

  	//printa o resultado da multiplicação - desativado por ser impossível checar os tempos com o programa imprimindo uma matriz 500x500 no processo.
  	
	for (int i = 0; i < dim; ++i)
	{
		printf("|");
		for (int j = 0; j < dim; ++j)
		{
			printf("%.2f  ", *(mat1+(dim*i)+j));
		}
		printf("|");
		printf("\t");
		printf("|");
		for (int j = 0; j < dim; ++j)
		{
			printf("%.2f  ", *(mat2+(dim*i)+j));
		}
		printf("|");
		printf("\t");
		printf("|");
		for (int j = 0; j < dim; ++j)
		{
			printf("%.2f  ", *(result+(dim*i)+j));
		}
		printf("|");
		printf("\n");
		printf("|");
	}
	

	GET_TIME(fim);
	
	elapsed = fim - parcial;
    printf("O tempo de exibição dos resultados foi de %lf segundos. \n", elapsed);
    
    elapsed = fim - inicio;
    printf("O tempo total de execução do programa foi de %lf segundos. \n", elapsed);

	return 0;
}