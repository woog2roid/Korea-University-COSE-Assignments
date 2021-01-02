#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void)
{
	FILE* R, * W;
	char read[80], write[80];
	char text[100];

	printf("Enter the name of file to read: ");
	scanf("%s", read);
	printf("Enter the name of file to write: ");
	scanf("%s", write);
	
	W = fopen(write, "w");

	if ((R = fopen(read, "r")) == NULL)
	{
		printf("File could not be opened");
	}
	else
	{
		while (!feof(R))
		{
			fgets(text, 100, R);
			if (text[0] == '\n')
			{
				;
			}
			else
			{

				fprintf(W, "%s", text);
			}

			for (int i = 0; i < 100; i++)
				text[i] = 0;
		}
		printf("Copy txt complete!");
		fclose(W);
		fclose(R);
	}
	
	return 0;
}