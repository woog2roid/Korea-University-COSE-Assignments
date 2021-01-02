#include <stdio.h>

void tower(int c, int start, int end, int temp);
//start=1, end=3, temp=2¹øÂ° ±âµÕ

int main(void)
{
	int num;
	printf("Enter the starting number of disks:");
	scanf("%d", &num);
	tower(num, 1, 3, 2);
}

void tower(int c, int start, int end, int temp)
{
	if (c == 1)
	{
		printf("%d --> %d \n", start, end);
		return;
	}

	tower(c - 1, start, temp, end);
	printf("%d --> %d \n", start, end);
	tower(c - 1, temp, end, start);

}