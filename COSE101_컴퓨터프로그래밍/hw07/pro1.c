	#include <stdio.h>
	#include <stdlib.h>


	int main(void)
	{
		char string[6];
		int integer[4];
		int sum = 0;

		for (int i = 0; i < 4; i++)
		{
			printf("Enter an integer string: ");
			scanf("%s", &string[0]);
			integer[i] = atoi(string);
			sum += integer[i];
		}
		printf("\n");

		printf("The total of the values is %d", sum);
	}

	//stringValue[6]