#include <stdio.h>

int main(void)
{
	int base, exponent, total=1;
	printf("Enter the base number:");
	scanf("%d", &base);
	while(base < 0)
	{
		printf("Enter the base number:");
		scanf_s("%d", &base);
	}
	
	printf("Enter the exponent number:");
	scanf("%d", &exponent);
	while (exponent < 0)
	{
		printf("Enter the exponent number:");
		scanf("%d", &exponent);
	}
	for (int i = 1; i <= exponent; i++)
	{
		total = total * base;
	}
	printf("%d^%d is %d", base, exponent, total);
}
