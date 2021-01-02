#include <stdio.h>

int add(int, int);
int sub(int, int);
int mul(int, int);

int main(void)
{
	int a, b;
	int(*func[3])(int, int) = { add, sub, mul };

	printf("0: Addition\n");
	printf("1:Subtraction\n");
	printf("2:Multiplication\n");
	printf("3:End\n");
	printf("select the operation:");
	
	int num;
	scanf("%d", &num);
	//¸ð¾ç

	switch (num)
	{
	case 0:
		printf("Input:");
		scanf("%d%d", &a, &b);
		printf("%d + %d = %d\n", a, b, func[0](a, b));
		break;
	
	case 1:
		printf("Input:");
		scanf("%d%d", &a, &b);
		printf("%d - %d = %d\n", a, b, func[1](a, b));
		break;
	
	case 2:
		printf("Input:");
		scanf("%d%d", &a, &b);
		printf("%d * %d = %d\n", a, b, func[2](a, b));
		break;
	
	case 3:
		return 0;
	}

}

int add(int a, int b)
{
	return a + b;
}
int sub(int a, int b)
{
	return a - b;
}
int mul(int a, int b)
{
	return a * b;
}