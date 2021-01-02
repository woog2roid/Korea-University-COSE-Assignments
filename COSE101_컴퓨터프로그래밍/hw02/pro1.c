#include <stdio.h>

int main(void)
{
	int num1, num2;
	printf("Enter two integers:");
	scanf_s("%d%d", &num1, &num2);
	int sum=0;
	printf("The sum of all integers between %d and %d is ", num1, num2);
	if (num1 <= num2) {
		while (num1 <= num2)
		{
			sum = sum + num1;
			num1++;
		}
	}
	else
	{
		while (num2 <= num1)
		{
			sum = sum + num2;
			num2++;
		}
	}
	printf("%d", sum);
	return 0;
}