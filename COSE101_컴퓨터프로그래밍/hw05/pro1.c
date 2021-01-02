#include <stdio.h>
#include <stdlib.h>
#include <time.h>




int main(void)
{

	int Array[20];
	srand(time(NULL));
	for (int i = 0; i < 20; i++)
	Array[i] = 1 + rand() % 20;
	
	/*
	for (int i = 0; i < 20; i++)
		printf("%d ", Array[i]);

	printf("\n");
	*/

	//È®ÀÎ¿ë 


	int tmp;
	for (int i = 0; i < 19; i++)
	{
		for (int j = i+1; j < 20; j++)
		{
			if (Array[i] == Array[j])
			{
				tmp = Array[i];
				for (int k = 0; k < 19; k++)
				{
					if (Array[k] == tmp)
					{
						Array[k] = 0;
					}
				}
			}
		}
	}

	
	for (int i = 0; i < 19; i++)
	{
		if (Array[i] == 0)
		{
			for (int j = i; j <= 18; j++)
				Array[j] = Array[j + 1];

			Array[19] = 30;
			i--;
		}
	}


	printf("Nonrepetitive array values are:\n");


	for (int i = 0; Array[i]!=30; i++)
	{
		printf("Array[ %d ] = %d\n", i, Array[i]);
	}
}