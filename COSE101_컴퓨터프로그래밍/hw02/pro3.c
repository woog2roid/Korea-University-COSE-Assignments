#include <stdio.h>

int main(void)
{
	printf("Enter a 5-digit number:");
	int num;
	scanf_s("%d", &num);
	
	if (num>99999)
	return 1;
	if (num<1)
	return 1;
	
	int i=1, a, numoriginal=num, result=0;
	while(i<=5)
	{
		a=num%10;
		num/=10;
		if(a==7)
		result++;
		i++;
	}
	printf("The number %d has %d seven(s)", numoriginal, result);
	return 0;
}
