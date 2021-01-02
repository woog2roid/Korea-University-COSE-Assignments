#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main(void)
{
	char str[3][80];
	printf("Enter three lines of text:\n");

	for (int i = 0; i < 3; i++)
		gets(&str[i][0]);

	char* result;

	int cnt[19];
	for (int i = 0; i < 19; i++)
	{
		cnt[i] = 0;
	}

	int size;

	for (int i = 0; i < 3; i++)
	{
		result = strtok(&str[i][0], ". \n");

		while (result != NULL)
		{
			size = strlen(result);
			cnt[size - 1]++;
			result = strtok(NULL, ". \n");
		}
	}

	for (int i = 1; i < 20; i++)
	{
		if (cnt[i - 1] == 0)
		{
			;
		}
		else if (cnt[i - 1] == 1)
		{
			printf("%d word of length %d\n", cnt[i - 1], i);
		}
		else
		{
			printf("%d words of length %d\n", cnt[i - 1], i);
		}
	}
}