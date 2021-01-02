		#include <stdio.h>

		void ulcase(char character);

		int main(void)
		{
			printf("Enter the Character : ");
			char character;
			scanf("%c", &character);
			ulcase(character);
		}

		void ulcase(char character)
		{
			if (character >= 65 && character <= 90)
			{
				printf("%c: uppercase", character);
			}
			else if (character >= 97 && character <= 122)
			{
				printf("%c: lowercase", character);
			}
			else
			{
				printf("INPUT IS NOT AN ALPHABET");
			}
		}
