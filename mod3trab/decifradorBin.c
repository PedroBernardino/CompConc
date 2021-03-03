#include <stdio.h>

int main()
{
	long long int nums;
	FILE *binario = fopen("teste.dat", "rb");


	fread(&nums, sizeof(long long int), 1, binario);
	printf("%lld\n", nums);


	for (int i = 0; i < nums; ++i)
	{
		int numero;
			fread(&numero, sizeof(int), 1, binario);
			printf("%d ", numero);
	}
	printf("\n");
	return 0;
}