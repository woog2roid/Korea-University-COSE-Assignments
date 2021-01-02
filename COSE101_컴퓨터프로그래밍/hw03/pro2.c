#include <stdio.h>

int main(void)
{
	printf("Decimal\tBinary\n");
	int number;
	for (number = 0; number <= 15; number++)
	{
		printf("%d\t", number);
		printf("%c", number < 16 && number >= 8 ? '1' : '0');
		printf("%c", number % 8 < 8 && number % 8 >= 4 ? '1' : '0');
		printf("%c", number % 4 < 4 && number % 4 >= 2 ? '1' : '0');
		printf("%c\n", number % 2 < 2 && number % 2 >= 1 ? '1' : '0');
	}
	return 0;
}
