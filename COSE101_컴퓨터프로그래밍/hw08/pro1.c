#include <stdio.h>


#define MAX 79

//구조체 설정
typedef struct class_grade
{
	char name[30];
	int student_id;
	int attendence;
	int midterm_exam;
	int final_exam;
	int assignment;
	int term_project;
	int total_score;
}Grade;

typedef struct class_report
{
	char name[30];
	int student_id;
	int attendence;
	int midterm_exam;
	int final_exam;
	int assignment;
	int term_project;
	int total_score;
}Report;

//함수
void go_back_to_menu(void);

void menu(void);
void menu_1(Grade array[]);
void menu_2(Grade array[]);
void menu_3(Grade array[], Report store[], int median[][MAX]);
void menu_4(Grade array[]);

//카운터 설정
int cnt = 0;

int main(void)
{
	//변수들 정리
	Grade array[MAX];
	Report store[2] = { 0 };
	int median[6][MAX];

	menu();

	int grade;
	while (1)
	{
		grade = getchar();

		switch (grade)
		{
		case '1':
			menu_1(array);
			break;
		case '2':
			menu_2(array);
			break;
		case '3':
			menu_3(array, store, median);
			break;
		case '4':
			menu_4(array);
			break;
		case '5':
			return 0;
		}
	}

}

void menu(void)
{
	printf("COSE101 Grade Management System\n");
	printf("================================\n");
	printf("1. View Registered Student\n");
	printf("2. Add Student\n");
	printf("3. Class Report\n");
	printf("4. View Final Grade\n");
	printf("5. Exit\n\n");
	printf("================================\n");
	printf("Select Menu  :");
}

void menu_1(Grade array[])
{
	system("cls");
	
	printf("================================\n");
	printf("       Registered Student       \n");
	printf("================================\n");
	if (cnt == 0)
		printf("There are no registered students\n");
	else
	{
		for (int i = 0; i < cnt; i++)
			printf("%d \t %s\n", array[i].student_id, &array[i].name[0]);
	}
	printf("================================\n");
	go_back_to_menu();
}

void menu_2(Grade array[])
{
	system("cls");

	printf("Student Name : ");
	scanf("%s", &array[cnt].name[0]);

	printf("Student ID : ");
	scanf("%d", &array[cnt].student_id);
	
	printf("Attendance (0 ~ 10) : ");
	scanf("%d", &array[cnt].attendence);
	while (array[cnt].attendence > 10 || array[cnt].attendence < 0)
	{
		printf("Attendance (0 ~ 10) : ");
		scanf("%d", &array[cnt].attendence);
	}

	printf("Assignment (0 ~ 10) : ");
	scanf("%d", &array[cnt].assignment);
	while (array[cnt].assignment > 10 || array[cnt].assignment < 0)
	{
		printf("Assignment (0 ~ 10) : ");
		scanf("%d", &array[cnt].assignment);
	}

	printf("Midterm Exam (0 ~ 30) : ");
	scanf("%d", &array[cnt].midterm_exam);
	while (array[cnt].midterm_exam > 30 || array[cnt].midterm_exam < 0)
	{
		printf("Midterm Exam (0 ~ 30) : ");
		scanf("%d", &array[cnt].midterm_exam);
	}

	printf("Fianl Exam (0 ~ 30) : ");
	scanf("%d", &array[cnt].final_exam);
	while (array[cnt].final_exam > 30 || array[cnt].final_exam < 0)
	{
		printf("Fianl Exam (0 ~ 30) : ");
		scanf("%d", &array[cnt].final_exam);
	}

	printf("Team Project (0 ~ 20) : ");
	scanf("%d", &array[cnt].term_project);
	while (array[cnt].term_project > 20 || array[cnt].term_project < 0)
	{
		printf("Team Project (0 ~ 20) : ");
		scanf("%d", &array[cnt].term_project);
	}

	printf("================================\n");

	array[cnt].total_score = array[cnt].attendence + array[cnt].assignment + array[cnt].midterm_exam + array[cnt].final_exam + array[cnt].term_project;

	cnt++;
	go_back_to_menu();
}

void menu_3(Grade array[], Report store[], int median[][MAX])
{
	system("cls");
	if (cnt != 0)
	{
		//평균
		store[0].attendence = 0;
		store[0].assignment = 0;
		store[0].midterm_exam = 0;
		store[0].final_exam = 0;
		store[0].term_project = 0;
		store[0].total_score = 0;


		for (int i = 0; i < cnt; i++)
		{
			store[0].attendence += array[i].attendence;
			store[0].assignment += array[i].assignment;
			store[0].midterm_exam += array[i].midterm_exam;
			store[0].final_exam += array[i].final_exam;
			store[0].term_project += array[i].term_project;
			store[0].total_score += array[i].total_score;
		}

		store[0].attendence /= cnt;
		store[0].assignment /= cnt;
		store[0].midterm_exam /= cnt;
		store[0].final_exam /= cnt;
		store[0].term_project /= cnt;
		store[0].total_score /= cnt;
//--------------------------------------------------------------------------------//	
		//중앙값

		for (int i = 0; i < cnt; i++)
		{
			median[0][i] = array[i].attendence;
			median[1][i] = array[i].assignment;
			median[2][i] = array[i].midterm_exam;
			median[3][i] = array[i].final_exam;
			median[4][i] = array[i].term_project;
			median[5][i] = array[i].total_score;
		}

		int tmp;
		for (int j = 0; j < 6; j++)
		{
			for (int pass = 0; pass < cnt; pass++)
			{
				for (int i = 0; i <= cnt - 2; i++)
				{
					if (median[j][i] > median[j][i + 1])
					{
						tmp = median[j][i];
						median[j][i] = median[j][i + 1];
						median[j][i + 1] = tmp;
					}
				}
			}
		}

		/*
			median[0][i] = array[i].attendence;
			median[1][i] = array[i].assignment;
			median[2][i] = array[i].midterm_exam;
			median[3][i] = array[i].final_exam;
			median[4][i] = array[i].term_project;
			median[5][i] = array[i].total_score;
		*/
		if (cnt % 2 == 1)
		{
				store[1].attendence = median[0][(cnt / 2)];
				store[1].assignment = median[1][(cnt / 2)];
				store[1].midterm_exam = median[2][(cnt / 2)];
				store[1].final_exam= median[3][(cnt / 2)];
				store[1].term_project = median[4][(cnt / 2)];
				store[1].total_score = median[5][(cnt / 2)];

		}
		else if (cnt % 2 == 0)
		{
			store[1].attendence = (median[0][(cnt / 2)]+ median[0][(cnt / 2)-1])/2;
			store[1].assignment = (median[1][(cnt / 2)]+ median[1][(cnt / 2)-1])/2;
			store[1].midterm_exam = (median[2][(cnt / 2)]+ median[2][(cnt / 2)-1])/2;
			store[1].final_exam = (median[3][(cnt / 2)]+ median[3][(cnt / 2)-1])/2;
			store[1].term_project = (median[4][(cnt / 2)]+ median[4][(cnt / 2)-1])/2;
			store[1].total_score = (median[5][(cnt / 2)] + median[5][(cnt / 2) - 1])/2;
		}
	
	}
	



	printf("               Class Report              \n");
	printf("=========================================\n");
	printf(" Field        Average    |    Median     \n");
	printf("=========================================\n");
	printf("Attendance     :  %3d    |  %3d    \n", store[0].attendence, store[1].attendence);
	printf("Assignment     :  %3d    |  %3d    \n", store[0].assignment, store[1].assignment);
	printf("Midterm        :  %3d    |  %3d    \n", store[0].midterm_exam, store[1].midterm_exam);
	printf("Final          :  %3d    |  %3d    \n", store[0].final_exam, store[1].final_exam);
	printf("Term Project   :  %3d    |  %3d    \n", store[0].term_project, store[1].term_project);
	printf("Total          :  %3d    |  %3d    \n", store[0].total_score, store[1].total_score);


	go_back_to_menu();
}

void menu_4(Grade array[])
{
	system("cls");

	printf("====================================\n");
	printf("            Final Grade             \n");
	printf("====================================\n");

	if (cnt == 0)
		printf("There are no registered students\n");
	else
	{
		printf(" StudentID    Total Score   Grade   \n");
		for (int i = 0; i < cnt; i++)
		{
			printf("%d    %-3d           ", array[i].student_id, array[i].total_score);
			//grade system
			if (array[i].total_score >= 90)	printf("A");
			else if (array[i].total_score >= 80 && array[i].total_score < 90)	printf("B");
			else if (array[i].total_score >= 70 && array[i].total_score < 80)	printf("C");
			else if (array[i].total_score < 70)	printf("D");

			printf("\n");
		}
	}
	
	printf("====================================\n");

	go_back_to_menu();
}

void go_back_to_menu(void)
{
	int num=0;
	while (num != 1)
	{
		printf("Enter 1 to Go Back :");
		scanf_s("\n%d", &num);
		if (num == 1)
		{
			system("cls");
			menu();
		}
	}
}