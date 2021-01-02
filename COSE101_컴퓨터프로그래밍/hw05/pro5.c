#include <Stdio.h>

void Sorting(int average[]);

int main(void)
{
	int score[5][5];
	int average[5] = { 0,0,0,0,0 };

	for (int i = 0; i < 5; i++)
	{
		printf("INDEX %d\n", i);
		printf("Enter 5 Quiz scores : ");
		for (int j = 0; j < 5; j++)
		{
			scanf("%d", &score[i][j]);
			average[i] += score[i][j];

		}
	}

	for (int i = 0; i < 5; i++)
	{
		average[i] /= 5;
	}

	
	Sorting(average);
}

void Sorting(int average[])
{


	int j, i, tmp;
	for ( j = 1; j < 5; j++)
	{
		for (i = 0; i < 4; i++)
		{
			if (average[i] < average[i + 1])
			{
				tmp = average[i];
				average[i] = average[i + 1];
				average[i + 1] = tmp;
			}
		}
	}
	printf("From Highest Scores To Lowest :\n");

	for (int i = 0; i < 5; i++)
		printf("%d ", average[i]);
}