#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void)
{
	int int_input1[6];
	int int_input2[6];
	int int_result[6];

	FILE* input1 = fopen("input1.txt", "r") ;
	for (int i = 0; !feof(input1); i++)
		fscanf(input1, "%d", &int_input1[i]);

	FILE* input2 = fopen("input2.txt", "r") ;
	for (int i = 0; !feof(input2); i++)
		fscanf(input2, "%d", &int_input2[i]);

	FILE* result = fopen("result.txt", "w");
	
	for (int i = 0; i < 6; i++)
	{
		int_result[i] = int_input1[i] + int_input2[i];
	}

	printf("Input1 matrix:\n");
	for (int i = 0; i < 3; i++)
		printf("%-5d", int_input1[i]);
	printf("\n");
	for (int i = 3; i < 6; i++)
		printf("%-5d", int_input1[i]);
	printf("\n");
	printf("\n");


	printf("Input2 matrix:\n");
	for (int i = 0; i < 3; i++)
		printf("%-5d", int_input2[i]);
	printf("\n");
	for (int i = 3; i < 6; i++)
		printf("%-5d", int_input2[i]);
	printf("\n");
	printf("\n");

	printf("result matrix:\n");
	for (int i = 0; i < 3; i++)
		printf("%-5d", int_result[i]);
	printf("\n");
	for (int i = 3; i < 6; i++)
		printf("%-5d", int_result[i]);

	for (int i = 0; i < 2; i++)
	{
		fprintf(result, "%d", int_result[i]);
		fprintf(result, "/");
	}
	fprintf(result, "%d\n", int_result[2]);
	for (int i = 3; i <5; i++)
	{
		fprintf(result, "%d", int_result[i]);
		fprintf(result, "/");
	}
	fprintf(result, "%d", int_result[5]);

	fclose(input1);
	fclose(input2);
	fclose(result);

	return 0;
}