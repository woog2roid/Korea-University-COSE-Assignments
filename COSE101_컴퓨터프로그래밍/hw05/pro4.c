#include <Stdio.h>
#include <stdlib.h>
#include <time.h>



int matrix1[5][6];

int matrix2[5][6];

int result[5][6];

void function(int matrix1[][6], int matrix[][6], int result[][6]);

int main(void)
{
	srand(time(NULL));
	for (int i = 0; i <5; i++)
		for (int j = 0; j < 6; j++)
		{
			matrix1[i][j] = 1 + rand() % 100;
		}


	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 6; j++)
		{
			matrix2[i][j] = 1 + rand() % 100;
		}

	function(matrix1, matrix2, result);
}

void function(int matrix1[][6], int matrix2[][6], int result[][6])
{
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 6; j++)
		{
			result[i][j] = matrix1[i][j] + matrix2[i][j];
		}

	printf("matrix1:");
	for (int i = 0; i < 5; i++)
	{
		printf("\n");
		for (int j = 0; j < 6; j++)
		{
			printf("%5d", matrix1[i][j]);
		}
	}
	printf("\n");

	printf("matrix2:");
	for (int i = 0; i < 5; i++)
	{
		printf("\n");
		for (int j = 0; j < 6; j++)
		{
			printf("%5d", matrix2[i][j]);
		}
	}
	printf("\n");

	printf("result:");
	for (int i = 0; i < 5; i++)
	{
		printf("\n");
		for (int j = 0; j < 6; j++)
		{
			printf("%5d", result[i][j]);
		}
	}
}