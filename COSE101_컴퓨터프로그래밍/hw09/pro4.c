#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

int main(void)
{
	FILE* fp;
	char file[80], word[80];
	char text[20];
	int word_cnt = 0, cnt = 0, check = 0;
	char* tokenPtr;

	printf("Enter the file name and word: ");
	scanf("%s", file);
	scanf("%s", word);

	if ((fp = fopen(file, "r")) == NULL)
	{
		printf("File could not be opened");
	}
	else
	{
		while (!feof(fp))
		{
			fscanf(fp, "%s", text);
			cnt++;

			for (int i = 0; text[i] != NULL; i++)
			{
				if (text[i] == '.' || text[i] == '"' || text[i] == ',')
				{
					for (int j = i; j < 20; j++)
					{
						text[j] = text[j + 1];
					}
					i--;
				}
			}


			if (!strcmp(text, word))
			{
				word_cnt++;
			}

		}

		printf("The finding ratio: %d/%d", word_cnt, cnt);
		fclose(fp);
	}



	//printf("%s", text);
	return 0;
}