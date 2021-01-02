#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main(void)
{
	char string[10][50];
	char tmp[50];

	for (int i = 0; i < 10; i++)
	{
		printf("Enter a string: ");
		gets(&string[i][0]);
	}

	for (int pass = 1; pass < 10; pass++)
	{
		for (int i = 0; i < 10; i++)
		{
			if (strcmp(&string[i][0], &string[i + 1][0]) == 1)
			{
				strcpy(tmp, &string[i][0]);
				strcpy(&string[i][0], &string[i + 1][0]);
				strcpy(&string[i + 1][0], tmp);
			}
		}
	}
	
	printf("The strings in sorted order are:\n");
	for (int i = 0; i < 10; i++)
	{
		printf("%s\n", &string[i][0]);
	}
}