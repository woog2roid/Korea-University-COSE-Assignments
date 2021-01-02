#include <stdio.h>

void stringReverse(char strArray[]);

int main(void)
{
	char strArray[30] = "Print this string backward.";
	printf("%s\n", strArray);
	stringReverse(strArray);
	return 0;
}

void stringReverse(char strArray[])
{
	static int cnt = 0;
	int num = cnt;
	if (strArray[cnt] != 0)
	{
	cnt++;
	stringReverse(strArray);
	printf("%c", strArray[num]);
	}
	else	
	{
		return;
	}
}
