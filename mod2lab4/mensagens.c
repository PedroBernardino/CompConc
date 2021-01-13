/* 
Programa: mensagens.c
Descricao: Envia mensagens de introdução e de complemento, sempre intercalando entre os tipos de mensagem
Autor: Pedro Henrique
*/



#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int intro = 1; //variável que decide se a próxima frase a ser escrita é de introdução ou não (complemento). 

pthread_mutex_t mutex; // variável de sincronização por exclusão mútua
pthread_cond_t cond_intro, cond_comp; //variáveis de sincronização das threads com mensagens introdutórias e de complemento.

//função que verifica se a mensagem da thread deve ser impressa agora ou posta em espera
void verificaProx(int introdutoria, int id) // variável que diz se a thread quer imprimir uma mensagem de introdução ou não.
{
	//printf("Thread %d tentando imprimir\n", id);
	pthread_mutex_lock(&mutex);
	//printf("%d é igual a %d?\n", intro,introdutoria);
	while(intro != introdutoria)
	{
		//printf("Thread %d na espera\n", id);
		if(introdutoria == 1) // se a thread for de uma mensagem introdutoria
			pthread_cond_wait(&cond_intro, &mutex); // coloca a thread em espera na fila de threads introdutórias
		else pthread_cond_wait(&cond_comp,&mutex); // coloca a thread em espera na fila de threads complementares
	}

	//printf("Thread %d conseguiu passar\n", id);
	// altera o sinal que diz qual o próximo tipo de mensagem que tem de ser impressa (introdutória ou complementar)
	if(intro == 1)
		intro = 0;
	else intro = 1;

	if(introdutoria == 1) // se a mensagem é introdutória, libera uma vaga para threads de mensagens complementares
		pthread_cond_signal(&cond_comp);
	else pthread_cond_signal(&cond_intro); // se a mensagem é complementar, libera uma vaga para threads de mengsagens introdutórias
	pthread_mutex_unlock(&mutex);
	
}


void * tudobem (void * arg)
{
	while(1)
	{
		int id = *((int *) arg);
		verificaProx(1, id); // verifica se essa thread pode imprimir agora ou não.
		printf("Tudo bem?\n"); // imprime a mensagem da thread na tela
		sleep(1); // pausa de 1 segundo até a próxima execução da thread
	}
	pthread_exit(NULL);
}

void * bomdia (void * arg)
{
	while(1)
	{
		int id = *((int *) arg);
		verificaProx(1, id); // verifica se essa thread pode imprimir agora ou não.
		printf("Bom Dia!\n"); // imprime a mensagem da thread na tela
		sleep(1); // pausa de 1 segundo até a próxima execução da thread
	}
	pthread_exit(NULL);
}

void * atemais (void * arg)
{
	while(1)
	{
		int id = *((int *) arg);
		verificaProx(0, id); // verifica se essa thread pode imprimir agora ou não.
		printf("Até mais!\n"); // imprime a mensagem da thread na tela
		sleep(1); // pausa de 1 segundo até a próxima execução da thread
	}
	pthread_exit(NULL);
}

void * boatarde (void * arg)
{
	while(1)
	{
		int id = *((int *) arg);
		verificaProx(0, id); // verifica se essa thread pode imprimir agora ou não.
		printf("Boa Tarde\n"); // imprime a mensagem da thread na tela
		sleep(1); // pausa de 1 segundo até a próxima execução da thread
	}
	pthread_exit(NULL);
}


int main(void)
{
	// vetor de ids das threads
	pthread_t tid[4]; 
	int t1 =1, t2 =2, t3 =3, t4=4;
 	//inicializa as variaveis de sincronizacao
 	pthread_mutex_init(&mutex, NULL);
 	pthread_cond_init(&cond_intro, NULL);
	pthread_cond_init(&cond_comp, NULL);


	if(pthread_create(&tid[1], NULL, tudobem, (void *) &t1)) exit(-1);
	if(pthread_create(&tid[2], NULL, bomdia, (void *) &t2)) exit(-1);
	if(pthread_create(&tid[3], NULL, atemais, (void *) &t3)) exit(-1);
	if(pthread_create(&tid[4], NULL, boatarde,(void *) &t4)) exit(-1);

	pthread_exit(NULL);
	return 0;
}