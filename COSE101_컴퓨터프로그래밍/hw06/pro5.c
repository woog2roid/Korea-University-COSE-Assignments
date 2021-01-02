#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 10

int main(void)
{
	srand(time(NULL));

	int arr[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		arr[i] = 1 + rand() % 100;
	}

	int* arrptr;
	arrptr = arr;
	
	for (int i = 0; i < SIZE; i++)
	{
		printf("%d  ", *arrptr);
		arrptr++;
	}

	printf("\n");

	for (int i = 0; i < SIZE; i++)
	{
		arrptr--;
		printf("%d  ", *arrptr);
	}
}