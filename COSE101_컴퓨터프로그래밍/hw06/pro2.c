#include <stdio.h>

int function(char string1[], char string2[]);

int main(void)
{
	printf("Enter two strings : ");
	char string1[80], string2[80];
	scanf("%s%s", string1, string2);
	
	if (function(string1, string2))
	{
		printf("%s and %s are equal.", string1, string2);
	}
	else
	{
		printf("%s and %s are not equal.", string1 , string2);
	}

}

int function(char string1[], char string2[])
{
	for (int i = 0; i < 80; i++)
	{
		if (string1[i] != string2[i])
		{
			return 0;
		}
	}
	return 1;
}
