#include <stdio.h>

int main(void)
{
	int edge;
	printf("Input the value of range edge :");
	scanf("%d", &edge);
	if (edge < 50 || edge>100)
	{
		printf("Wrong Input! range edge should be from 50~100");
		return 1;
	}
		int i = 1;
	do
	{
		printf("%d\t\t", i);
		int mod, quo;
		quo = i / 10;
		mod = i % 10;
		i++;
		switch (quo)
		{
		case 0:
			break;
		case 1:
			printf("X");
			break;
		case 2:
			printf("XX");
			break;
		case 3:
			printf("XXX");
			break;
		case 4:
			printf("XL");
			break;
		case 5:
			printf("L");
			break;
		case 6:
			printf("LX");
			break;
		case 7:
			printf("LXX");
			break;
		case 8:
			printf("LXXX");
			break;
		case 9:
			printf("XC");
			break;
		case 10:
			printf("C");
			break;
		}

		switch (mod)
		{
		case 0:
			printf("\n");
			break;
		case 1:
			printf("l\n");
			break;
		case 2:
			printf("ll\n");
			break;
		case 3:
			printf("lll\n");
			break;
		case 4:
			printf("IV\n");
			break;
		case 5:
			printf("V\n");
			break;
		case 6:
			printf("VI\n");
			break;
		case 7:
			printf("VII\n");
			break;
		case 8:
			printf("VIII\n");
			break;
		case 9:
			printf("IX\n");
			break;
		}
	}while (i <= edge);
	return 0;
}
