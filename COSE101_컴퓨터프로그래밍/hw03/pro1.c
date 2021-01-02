#include <stdio.h>

int main(void)
{
	int a, largest=0;
	int i=1;
	do
	{
		printf("Enter the number:");
		scanf("%d", &a);
		if (i==1)
		{
			largest=a;
		}
		if (a>=largest)
		{
			largest=a;
		}
		i++;
	}while(i<=5);
	printf ("Largest is %d", largest);
}
