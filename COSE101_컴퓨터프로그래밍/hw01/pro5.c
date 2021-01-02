#include <stdio.h>

int main(void)
{
	printf("Input:");
	int num;
	scanf_s("%d", &num);
	int a = num % 10;
	int b = num / 10 % 10;
	int c = num / 100 % 10;
	int d = num / 1000 % 10;
	int e = num / 10000;
	printf("Output:\n%d   %d   %d   %d   %d\n", a, b, c, d, e);
	int w1=0, w2=0, w3=0, w4=0, w5=0;
	if(a%2==1)
	{w1=a;}
	if(b%2==1)
	{w2=b;}
	if(c%2==1)
	{w3=c;}
	if(d%2==1)
	{w4=d;}
	if(e%2==1)
	{w5=e;}
	printf("%d", w1+w2+w3+w4+w5);
	
	return 0;
}
