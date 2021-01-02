#include <stdio.h>

void function(char string1[], char string2[]);

int main(void)
{
	printf("Enter two strings:");
	char string1[80], string2[80];

	scanf("%s%s", string1, string2);

	function(string1, string2);
}

void function(char string1[], char string2[])
{
	int i;
	for (i = 0; string1[i] != 0; i++)
	{;}

	int j;
	for (j = 0; string1[j] != 0; j++)
	{;}

	for (int k = 0; k < j; k++)
		string1[i + k] = string2[k];

	for (int p = 0; p <i+j ; p++)
		printf("%c", string1[p]);
}