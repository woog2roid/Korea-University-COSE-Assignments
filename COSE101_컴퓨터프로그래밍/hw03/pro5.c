#include <stdio.h>

int main(void)
{
	printf("Enter pairs of item numbers and quantities.\nEnter -1 for the item number to end input");
	int a=1, b;
	int cnt1=0, cnt2=0, cnt3=0, cnt4=0, cnt5=0;
	while (1)
	{
		scanf_s("%d", &a);
		if (a == -1)
			break;
		scanf_s("%d", &b);
		switch (a)
		{
		case 1:
			cnt1 += b;
			break;
		case 2:
			cnt2 += b;
			break;
		case 3:
			cnt3 += b;
			break;
		case 4:
			cnt4 += b;
			break;
		case 5:
			cnt5 += b;
			break;
		default :
			printf("Invalid product code: %d\n", a);
			printf("            Quantity: %d", b);
			break;
		}
	}
	printf("The total retail value was: %.2f", 2.98*cnt1 + 4.50*cnt2 + 9.98*cnt3 + 4.49*cnt4 + 6.87*cnt5);
}