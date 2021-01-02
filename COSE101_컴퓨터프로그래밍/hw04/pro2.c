#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int flip(void);

int main(void)
{
	int countH=0;
	int countT=0;
	srand(time(NULL));
	for (int i=1; i <= 100; i++)
	{
		if (flip() == 0)
		{
			printf("Tails  ");
			countT++;
		}
		else
		{
			printf("Heads  ");
			countH++;
		}
		if (i % 10 == 0)
		{
			printf("\n");
		}
	}
	printf("The total number of Heads was %d\n", countH);
	printf("The total number of Tails was %d", countT);
}

int flip(void)
{
	int num = rand();
	int num2 = num % 2;
	return num2;
}
