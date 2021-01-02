#include <stdio.h>

int main(void)
{	
	int i = 1;
	while(i<=4)
	{
		printf("Enter Employee %d's sales in dollars:", i);
		int num, total;
		scanf_s("%d", &num);
		if (num >= 3000) {
			total = 200 + num * 0.09;
			printf("Employee %d's salary is %d\n\n", i, total);
		}
		else {
			total = 200;
			printf("Employee %d's salary is %d\n\n", i, total);
		}
		i++;
	}
	return 0;
}