#include <stdio.h>
#include <stdlib.h>



int main(int argc, char const *argv[])
{
	long long int nums;
	FILE *arquivoTeste;
	FILE *binario = fopen("teste.dat", "wb");
	int *sequenciaOriginal;

	if ( argc != 3 ) 
	{
        printf( "Favor digitar o nome do arquivo a ser executado , o arquivo de entrada dos números e a quantidade de repetições do arquivo original devem ser impressas\n");
        return -1;
	}
	
   arquivoTeste = fopen( argv[1], "r" );
   int numeroRepeticoes = atoi(argv[2]);

	if (arquivoTeste == NULL)
	{
    	printf( "Erro na abertura do arquivo\n" );
    	return -2;
	}

	fscanf(arquivoTeste, "%lld", &nums);
	sequenciaOriginal = (int *) malloc (nums * sizeof (int));
	long long int numerosRepetidos = numeroRepeticoes*nums;
	printf("%lld\n", numerosRepetidos);
	fwrite(&numerosRepetidos, sizeof(long long int), 1, binario);


	for (int i = 0; i < nums; ++i)
		fscanf(arquivoTeste, "%d", &sequenciaOriginal[i]);

	printf("O número original é %lld,o numéro de repetições é %d e a sequencia original é:\n", nums, numeroRepeticoes);
	for (int i = 0; i < nums; ++i)
	{
		printf("%d \n", sequenciaOriginal[i]);
	}


	for (int i = 0; i < numeroRepeticoes; ++i)
		for (int j = 0; j < nums; ++j)
			fwrite(&sequenciaOriginal[j], sizeof(int), 1, binario);

	return 0;
}