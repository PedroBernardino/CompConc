#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

#define N 5 //tamanho do buffer
#define M 10 //tamanho de cada bloco do buffer
int buffer[N][M], blocoLido[N][3]; //buffer que armazena os blocos, e matriz que guarda a informação de quais blocos ja foram lidos pelas 3 threads consumidoras
int proxt0=0,proxt1=0,proxt2=0,proxleitor=0; // marca o próximo bloco a ser lido pelas 3 threads consumidoras e a thread leitora
long long int numerosRestantes; // quantidade de números a serem lidos do array
pthread_cond_t produtor, consumidores; //variáveis de condição
pthread_mutex_t mutex; //exclusão mútua


//thread responsável pela leitura do arquivo
void * leituraArquivo(void * arg)
{
	FILE *arquivo = (FILE *) arg; //recebe como parametro o arquivo binário para leitura
	while(1)
	{
		pthread_mutex_lock(&mutex);
		//bloqueia a thread caso a próxima casa do buffer a ser reescrita ainda não tenha sido lida por todas as threads

		while(blocoLido[proxleitor][0]==0 ||
			blocoLido[proxleitor][1] == 0 ||
			blocoLido[proxleitor][2] == 0) 
			pthread_cond_wait(&produtor, &mutex);

		pthread_mutex_unlock(&mutex);
		for (int i = 0; i < M; ++i)
		{
			//lê um número do arquivo e armazena no bloco atual do buffer
			fread(&buffer[proxleitor][i], sizeof(int), 1, arquivo);
			numerosRestantes--;

			if(numerosRestantes == 0) // se não houverem mais números a serem lidos
			{

				if(i<M-1) //se os números acabarem antes do final do bloco, preenche a casa após o fim com -1
				{
					buffer[proxleitor][i+1] = -1;
					i = M;
				}

				else //caso contrário, encerra o bloco, abre uma nova requisição pelo próximo bloco apenas pra trocar a primeira casa por -1
				{
					pthread_mutex_lock(&mutex);
					blocoLido[proxleitor][0] = 0;
					blocoLido[proxleitor][1] = 0;
					blocoLido[proxleitor][2] = 0;
					pthread_mutex_unlock(&mutex);
					proxleitor = (proxleitor+1) % N;
					pthread_cond_broadcast(&consumidores);
					while(blocoLido[proxleitor][0]==0 || blocoLido[proxleitor][1] == 0 || blocoLido[proxleitor][2] == 0) 
						pthread_cond_wait(&produtor, &mutex);
					buffer[proxleitor][0] = -1;
					pthread_mutex_unlock(&mutex);
				}
			}
		}
		//marca o bloco reescrito como não lido pelas threads consumidoras
		pthread_mutex_lock(&mutex);
		blocoLido[proxleitor][0] = 0;
		blocoLido[proxleitor][1] = 0;
		blocoLido[proxleitor][2] = 0;
		pthread_mutex_unlock(&mutex);

		proxleitor = (proxleitor+1) % N;
		pthread_cond_broadcast(&consumidores);
		if(numerosRestantes == 0) pthread_exit(NULL); //se os números acabaram, a thread já encerrou a leitura, basta sair

	}
}





void * MaiorSequencia()
{
	int numAtual = -1, seqAtual= 0,posSequenciaAtual = 0;
	int maiorSequencia = 0, numMaiorSequencia = -1, posMaiorSequencia = 0;
	int numLidos = 0;
	int *vetor;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(blocoLido[proxt0][0] == 1) //se o próximo bloco a ser lido ainda não foi reescrito pelo produtor
			pthread_cond_wait(&consumidores, &mutex);

		pthread_mutex_unlock(&mutex);

		//pega um vetor(bloco) de inteiros de tamanho M do buffer para ler

		vetor = buffer[proxt0]; // apenas para facilitar a compreensão do código, variável que aponta pro bloco sendo lido atualmente
		for(int i = 0; i < M; i++)
		{
			numLidos++;
			
			if(vetor[i] == numAtual)// se o número lido agora for igual ao último número lido
				seqAtual++;
			else{
				if(seqAtual > maiorSequencia) //se a sequência que acabou agora for maior que a última maior sequência registrada
				{
					maiorSequencia = seqAtual;
					numMaiorSequencia = numAtual;
					posMaiorSequencia = posSequenciaAtual;
				}
				numAtual = vetor[i];
				seqAtual = 1;
				posSequenciaAtual = numLidos;
			}

			if(vetor[i] == -1)//acabaram os números a serem lidos
			{
				pthread_mutex_lock(&mutex);
				printf("Maior Sequencia de valores idênticos: %d %d %d\n", posMaiorSequencia, maiorSequencia, numMaiorSequencia);
				pthread_mutex_unlock(&mutex);
				pthread_exit(NULL);
			}	
		}

		pthread_mutex_lock(&mutex);
		blocoLido[proxt0][0] = 1; //assinala que leu o bloco do vetor
		pthread_mutex_unlock(&mutex);
		proxt0 = (proxt0 + 1) % N; //marca a próxima posição do buffer a ser consumida
		pthread_cond_signal(&produtor); //libera a thread leitora do arquivo caso esteja bloqueada
	} 
}





void * SequenciaDe3()
{
	int cont = 0, sequencias = 0, numAtual = -1;
	int *vetor;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(blocoLido[proxt1][1] == 1) //se o próximo bloco a ser lido ainda não foi reescrito pelo produtor
		{
			pthread_cond_wait(&consumidores, &mutex);
		}
		pthread_mutex_unlock(&mutex);

		//pega um vetor(bloco) de inteiros de tamanho M do buffer para ler
		vetor = buffer[proxt1]; // apenas para facilitar a compreensão do código, variável que aponta pro bloco sendo lido atualmente

		for(int i = 0; i < M; i++)
		{
			if(vetor[i] == -1)//acabaram os números a serem lidos
			{
				pthread_mutex_lock(&mutex);
				printf("Quantidade de triplas: %d\n", sequencias);
				pthread_mutex_unlock(&mutex);
				pthread_exit(NULL);
			}	


			if(vetor[i] == numAtual)
			{
				cont++;
				if(cont == 3)
				{
					sequencias++;
					cont = 0;
				}
			}
			else{
				numAtual = vetor[i];
				cont = 1;
			}
		}

		pthread_mutex_lock(&mutex);
		blocoLido[proxt1][1] = 1; //assinala que leu o bloco do vetor
		pthread_mutex_unlock(&mutex);
		proxt1 = (proxt1 + 1) % N; //marca a próxima posição do buffer a ser consumida
		pthread_cond_signal(&produtor); //libera a thread leitora do arquivo caso esteja bloqueada
	}
}


void * SequenciaCrescente()
{
	int numAtual = -1, sequencias = 0;
	int *vetor;
	while(1)
	{

		pthread_mutex_lock(&mutex);
		while(blocoLido[proxt2][2] == 1) //se o próximo bloco a ser lido ainda não foi reescrito pelo produtor
			pthread_cond_wait(&consumidores, &mutex);

		pthread_mutex_unlock(&mutex);


		//pega um vetor(bloco) de inteiros de tamanho M do buffer para ler
		vetor = buffer[proxt2]; // apenas para facilitar a compreensão do código, variável que aponta pro bloco sendo lido atualmente
		for(int i = 0; i < M; i++)
		{

			if(vetor[i] == -1)//acabaram os números a serem lidos
			{
				pthread_mutex_lock(&mutex);
				printf("Quantidade de ocorrências da sequência <012345>: %d\n", sequencias);
				pthread_mutex_unlock(&mutex);
				pthread_exit(NULL);
			}	

			//a sequência começa em -1 e atualiza sempre que ler um número maior em 1 que o seu valor atual. Se chegar a 5, conta uma sequencia <012345>
			if(vetor[i] == numAtual + 1)
			{
				numAtual++;
				if(numAtual == 5)
				{
					sequencias++;
					numAtual = -1;
				}
			}
			else numAtual = -1;
		}

		pthread_mutex_lock(&mutex);
		blocoLido[proxt2][2] = 1; //assinala que leu o bloco do vetor
		pthread_mutex_unlock(&mutex);
		proxt2 = (proxt2 + 1) % N; //marca a próxima posição do buffer a ser consumida
		pthread_cond_signal(&produtor); //libera a thread leitora do arquivo caso esteja bloqueada
	}
}



int main(int argc, char *argv[])
{
	//variáveis iniciais
	double inicio, fim;
	pthread_t tid[4]; //vetor de id das threads
	FILE *arq;

	//inicialização das variáveos de condição e exclusão mútua
	pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&produtor, NULL);
    pthread_cond_init(&consumidores,NULL);

	if ( argc != 2 ) 
	{
        printf("Favor digitar o nome do arquivo a ser executado e o arquivo de entrada dos números\n");
		return -1;
	}
	
	//abre o arquivo binário
	arq = fopen( argv[1], "rb" );

    if (arq == NULL)
    {
        printf( "Erro na abertura do arquivo\n" );
        return -2;
    }

    //le a quantidade de termos da sequência
    fread(&numerosRestantes,sizeof(long long int),1,arq);

//no ínicio do programa, marca todos os blocos do buffer como "lidos", para que a thread produtora possa reescrevê-los
    for (int i = 0; i < N; ++i)
		for (int j = 0; j < M; ++j)
    		blocoLido[i][j] = 1; 

    //começa a contar o tempo de solução do programa
    GET_TIME(inicio);


    //cria as threads
    pthread_create(&tid[0], NULL, leituraArquivo, (void *) arq);
    pthread_create(&tid[1], NULL, MaiorSequencia, NULL );
    pthread_create(&tid[2], NULL, SequenciaDe3, NULL);
    pthread_create(&tid[3], NULL, SequenciaCrescente, NULL);
    
    //permite as threads trabalhando terminarem
    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
    pthread_join(tid[2],NULL);
    pthread_join(tid[3],NULL);

    //termina a contagem de tempo do programa
    GET_TIME(fim);

    printf("O tempo de execução do programa foi de %.3lf segundos \n", fim-inicio);


	return 0;
}