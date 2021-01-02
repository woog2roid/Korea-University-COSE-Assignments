#include <Stdio.h>

void i_to_a(int num);

int main(void)
{
	int num;
	while (1)
	{
		printf("Enter the inputs : ");
		scanf_s("%d", &num);

		if (num < 100000 || num>999999)
		{
			printf("Input Error!\n\n");
		}
		else
		{
			break;
		}
	}
	i_to_a(num);
}
//97~122 0~25
void i_to_a(int num)
{
	int numa;
	static int i = 1;
	numa = num / 10000;
	numa %= 26;
	num = (num % 10000) * 100;
	printf("%c  ", numa + 'A');
	i++;
	if (i <= 3)
	{
		i_to_a(num);
	}
}