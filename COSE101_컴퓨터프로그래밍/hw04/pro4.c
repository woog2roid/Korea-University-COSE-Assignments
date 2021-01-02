#include <stdio.h>

long long factorial(int num);
long combination(int n, int r);

int main(void)
{
	int n, r;
	while (1)
	{
		printf("Enter the inputs : ");
		scanf_s("%d%d", &n, &r);

		if (!(n >= 5 && n <= 20 && r >= 5 && r <= 20 && n >= r))
			printf("Input Error! \n\n");

		if (n >= 5 && n <= 20 && r >= 5 && r <= 20 && n >= r)
			break;
	}
	printf("nCr : %d", combination(n, r));
}

long long factorial(int num)
{
	if (num <= 1)
	{
		return 1;
	}
	else
	{
		return (num * factorial(num - 1));
	}
}

long combination(int n, int r)
{
	return (factorial(n) / (factorial(r) * factorial(n - r)));
}
