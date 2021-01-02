#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void encryptor(char words[][10], char output[]);

void decryptor(char words[][10], char output[]);

//craftbeer, metallica, whitewine, champagne, bourgogne, sandiego, lafayette, pinotnoir, sauvignon, dondiablo

int main(void)
{

	char words[10][10];	char output[10] = {0};

	strcpy(&words[0][0], "craftbeer");
	strcpy(&words[1][0], "metallica");
	strcpy(&words[2][0], "whitewine");
	strcpy(&words[3][0], "chamapgne");
	strcpy(&words[4][0], "bourgogne");
	strcpy(&words[5][0], "sandiego");
	strcpy(&words[6][0], "lafayette");
	strcpy(&words[7][0], "pinotnoir");
	strcpy(&words[8][0], "sauvignon");
	strcpy(&words[9][0], "dondiablo");


	encryptor(words, output);
	decryptor(words, output);
}

void encryptor(char words[][10], char output[])
{
	srand(time(NULL));
	int key = 1 + rand() % 19;

	int i = rand() % 10;

	printf("              -Encryptor-\n");
	printf("Selected word:  %s\n", &words[i][0]);
	printf("Selected value of the key: %d\n", key);

	for (int j = 0; j < 9; j++)
	{
		output[j] = words[i][j] + key;
	}

	printf("Ciphertext: %s\n\n", output);
}	

void decryptor(char words[][10], char output[])
{
	printf("              -Decoder-\n");
	printf("Ciphertext input: %s\n\n", output);

	char tmp[10];

	for (int i = 1; i <= 19; i++)
	{
		strcpy(tmp, output);
		printf("> Guess %d <\n  Key: %d\n", i, i);

		for (int j = 0; j < 9; j++)
		{
			tmp[j] -= i;
		}
		printf("  Plaintext: %s\n", tmp);

		for (int k = 0; k < 10; k++)
		{
			if (strcmp(&words[k][0], tmp) == 0)
			{
				printf("Found the correct answer!\n\n");
				printf("result : '%d' was selected as the Key.\n", i);
				printf("       '%s' was the Plaintext.", tmp);
				
				return 0;
			}
		}
		printf("Matching word not found\n\n");
	}

}