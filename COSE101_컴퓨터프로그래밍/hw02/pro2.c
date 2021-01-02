#include <Stdio.h>

int main(void)
{
	printf("Enter a positive integer:");
	int num, result=1;
	scanf_s("%d", &num);
	while (num < 0)
	{
		printf("Enter a positive integer:");
		scanf_s("%d", &num);
	}
	printf("%d! is ", num);
	while (num > 0)
	{
		result = result * num;
		num--;
	}
	printf("%d", result);
	return 0;
}
