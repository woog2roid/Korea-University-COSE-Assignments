#include <stdio.h>

int main(void)
{
	printf("Input the integers:");
	int num1, num2;
	scanf_s("%d%d", &num1, &num2);
	if (num1%num2 == 0) {
		int num3 = num1 / num2;
		printf("%d is a multiple of %d by a facor of %d",num1, num2, num3 );
	}
	else {
		printf("%d is not a multiple of %d", num1, num2);
	}
	return 0;
}