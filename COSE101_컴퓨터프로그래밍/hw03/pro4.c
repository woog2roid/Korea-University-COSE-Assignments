#include <stdio.h>
int main(void)
{
	for (int i = 1; i <= 9; i += 2)
	{
		for(int a=1; a<=(9-i)/2; a++)
		printf(" ");
		for (int a = 1; a <= i; a++)
		printf("*");
		for (int a = 1; a <= (9 - i) / 2; a++)
		printf(" ");

		printf("\n");
	}
	for (int i = 7; i >= 1; i -= 2)
	{
		for (int a = 1; a <= (9 - i) / 2; a++)
			printf(" ");
		for (int a = 1; a <= i; a++)
			printf("*");
		for (int a = 1; a <= (9 - i) / 2; a++)
			printf(" ");

		printf("\n");
	}
	return 0;
}