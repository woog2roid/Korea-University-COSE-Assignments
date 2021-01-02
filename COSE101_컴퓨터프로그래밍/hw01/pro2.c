#include <stdio.h>

int main(void)
{
	int num1, num2;
	printf("Enter two numbers.");
	scanf_s("%d%d", &num1, &num2);
	printf("The sum is %d.\n", num1 + num2);
	printf("The product is %d\n", num1*num2);
	printf("The difference is %d\n", num1 - num2);
	printf("The quotient is %d\n", num1 / num2);
	printf("THe remainder is %d\n", num1 % num2);
	return 0;
}
