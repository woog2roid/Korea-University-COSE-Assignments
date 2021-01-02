#include <stdio.h>

int main(void)
{
	int num;
	printf("input an integer:");
	scanf_s("%d", &num);
	if (num % 2 == 1)
	{
		printf("%d is an odd integer.", num);
	}
	if (num % 2 == 0)
	{
		printf("%d is an even integer.", num);
	}
	return 0;
}
