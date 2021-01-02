#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <string.h>

typedef struct _Admin
{
	char name[20];
	int student_number;
	int grade;
	char major[20];
} ADMIN;

ADMIN student[5];

int main(void)
{
	FILE* filePtr = fopen("student_file.txt", "w");

	for (int i = 0; i < 5; i++)
	{
		printf("Name Student_number Grade Major: ");
		scanf("%s", student[i].name);
		scanf("%d", &student[i].student_number);
		scanf("%d", &student[i].grade);
		scanf("%s", student[i].major);

		fprintf(filePtr, "%s %d %d %s\n", student[i].name, student[i].student_number, student[i].grade, student[i].major);
	}
	//텍스트파일에 등록 완료
	fclose(filePtr);

	int srch;
	printf("Insert the student number to find: ");
	scanf("%d", &srch);

	filePtr = fopen("student_file.txt", "r");
	char name[20];
	int student_number;
	int grade;
	char major[20];
	fscanf(filePtr, "%s %d %d %s", name, &student_number, &grade, major);
	
	
	//일치하는 지 확인만 해볼까?????


	


	while (!feof(filePtr))
	{
		if (srch == student_number)
		{
			printf("The major of student with ID %d is %s", srch, major);
		}
		fscanf(filePtr, "%s %d %d %s", name, &student_number, &grade, major);
	}

	fclose(filePtr);
	return 0;
	
}