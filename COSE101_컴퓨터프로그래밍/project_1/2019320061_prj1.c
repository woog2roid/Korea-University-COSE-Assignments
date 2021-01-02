#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//함수 프로토타입

//모양
void border_top(int num);//뚜껑
void border_middle1(int num);//중간
void border_middle2(int num);//중간
void border_connection(int num);//연결고리
void border_bottom(int num);//밑바닥

// 화면
void first_main(void);//메인
void case1_display(void);//case1, 요금제 설명(input 필요 x)
void case2_display(int plan, int voice[], int text[], float data[]);//case2, 랜덤하게 이전 2달 설정해서 보여줌
void case3_display(int voice[], int text[], float data[]);//case3, 이번달 사용량 입력
void case4_display(int plan, int voice[], int text[], float data[], float extra_data[], int fee[]);//case4, 3달동안 요금
void case5_display(int plan, int voice[], int text[], float data[]);//case5, 평균내서 요금보여주고 추천, extra data상관 XXXXX, 그냥 계산

//기능
void back_to_main(void);

void extra_data_cal(int plan, float data[] ,float extra_data[]);

void fee_cal_basic(int voice[], int text[], float data[], int fee[], int num, float extra_data[]);
void fee_cal_more_data(int voice[], int text[], float data[], int fee[], int num, float extra_data[]);


//메인함수 시작
int main(void)
{

	//변수설정

	//(랜덤:플랜, 통화, 텍스트, 데이터)	
	srand(time(NULL));

	int plan = 1 + rand() % 2; //플랜 1:Basic 2:More Data
	int voice[4] = { 1 + rand() % 700,  1 + rand() % 700 }; //통화
	int text[4] = { rand() % 301, rand() % 301 }; //텍스트
	float data[4] = { 0.50 + (float)(rand() % 14951) / 100, 0.50 + (float)(rand() % 14951) / 100 };//데이터
	//0:첫달 1:둘째달 2:셋째달 3:평균

	float extra_data[3];
	//0,1,2 1,2.3
	int fee[3];
	//	int average_fee[2]; 이거는 case5_display에 저장되어있음, 특별히 호출이 필요가 없기에

	first_main();//메인화면

	while (1)
	{
		char c;
		scanf("%c", &c);  //input

		switch (c)
		{
		case '1':
			case1_display();
			back_to_main();
			break;

		case '2':
			case2_display(plan, voice, text, data);
			back_to_main();
			break;

		case '3':
			case3_display(voice, text, data);
			back_to_main();
			break;

		case '4':
			case4_display(plan, voice, text, data, extra_data, fee);
			back_to_main();
			break;

		case '5':
			case5_display(plan, voice, text, data);
			back_to_main();
			break;

		case '6':
			return 0;

		default:
			break;
		}
	}
}
//메인 끝


//모양
void border_top(int num) 
{
	printf("%c", 1);
	for (int i = 1; i <= num; i++)
		printf("%c", 6);
	printf("%c\n", 2);
}

void border_middle1(int num)
{
	printf("%c", 5);
	for (int i = 1; i <= num; i++)
		printf(" ");
}

void border_middle2(int num)
{
	for (int i = 1; i <= num; i++)
		printf(" ");
	printf("%c\n", 5);
}

void border_connection(int num)
{
	printf("%c", 25);
	for (int i = 1; i <= num; i++)
		printf("%c", 6);
	printf("%c\n", 23);
}

void border_bottom(int num)
{
	printf("%c", 3);
	for (int i = 1; i <= num; i++)
		printf("%c", 6);
	printf("%c\n", 4);
}


//기능
void back_to_main(void)
{
	char A;
	printf("\nBack to the main menu Y/N : ");
	while (1)
	{
		scanf("\n%c", &A);
		if (A == 'Y' || A == 'y')
		{
			break;
		}
		else if (A == 'N' || A == 'n')
		{
			printf("\nBack to the main menu Y/N : ");
		}
		else
		{
			printf("\nInput Error!\n");
			printf("Back to the main menu Y/N : ");
		}
	}
	system("cls");
	first_main();
}

void fee_cal_basic(int voice[], int text[], float data[], int fee[], int num, float extra_data[])
{
	if (voice[num] > 500) fee[num] += (voice[num] - 500) * 50;
	if (text[num] > 100)  fee[num] += (text[num] - 100) * 10;
	if (num == 0)
	{
		if (data[num] > 2.00) fee[num] += (data[num] - 2.00) * 10000;
	}
	else 
	{
		if (data[num] > 2.00+extra_data[num-1]) fee[num] += (data[num] - extra_data[num - 1] - 2.00) * 10000;
	}
}

void fee_cal_more_data(int voice[], int text[], float data[], int fee[], int num, float extra_data[])
{
	if (voice[num] > 300) fee[num] += (voice[num] - 300) * 10;
	if (text[num] > 100)  fee[num] += (text[num] - 100) * 30;
	if (num == 0)
	{
		if (data[num] > 30.00) fee[num] += (data[num] - 30.00) * 5000;
	}
	else
	{
		if (data[num] > 30.00+extra_data[num-1]) fee[num] += (data[num] - extra_data[num-1] - 30.00) * 5000;
	}
}

void extra_data_cal(int plan, float data[], float extra_data[])
{
	if (plan == 1)//basic이면,
	{
		extra_data[0] = 2.00 - data[0];
		if (extra_data[0] >= 0.20)	extra_data[0] = 0.20;
		if (extra_data[0] < 0) extra_data[0] = 0.00;
		
		extra_data[1] = 2.00 + extra_data[0] - data[1];
		if (extra_data[1] >= 0.20)	extra_data[1] = 0.20;
		if (extra_data[1] < 0) extra_data[1] = 0.00;

		extra_data[2] = 2.00 + extra_data[1] - data[2];
		if (extra_data[2] >= 0.20)	extra_data[2] = 0.20;
		if (extra_data[2] < 0) extra_data[2] = 0.00;
	}
	else//more data plan이면,
	{
		extra_data[0] = 30.00 - data[0];
		if (extra_data[0] >= 3.00)	extra_data[0] = 3.00;
		if (extra_data[0] < 0) extra_data[0] = 0.00;

		extra_data[1] = 30.00 + extra_data[0] - data[1];
		if (extra_data[1] >= 3.00)	extra_data[1] = 3.00;
		if (extra_data[1] < 0) extra_data[1] = 0.00;

		extra_data[2] = 30.00 + extra_data[1] - data[2];
		if (extra_data[2] >= 3.00)	extra_data[2] = 3.00;
		if (extra_data[2] < 0) extra_data[2] = 0.00;
	}
}


//화면


void first_main(void)
{
	//첫화면 제작하기!!
	border_top(50);
	border_middle1(9);
	printf("KU Mobile Phone Bill Calculator");
	border_middle2(10);
	border_connection(50);
	

	border_middle1(3);
	printf("1 : Display plans");
	border_middle2(30);

	border_middle1(3);
	printf("2 : Usage information for past two months");
	border_middle2(6);

	border_middle1(3);
	printf("3 : Input this month usage");
	border_middle2(21);

	border_middle1(3);
	printf("4 : Total fee for past 3 months");
	border_middle2(16);

	border_middle1(3);
	printf("5 : Best paln recommendation");
	border_middle2(19);

	border_middle1(3);
	printf("6 : Exit");
	border_middle2(39);

	border_bottom(50);
	//화면 제작 완료!!!
}

void case1_display(void)
{
	system("cls");

	border_top(50);
	border_middle1(20);
	printf("Basic Plan");
	border_middle2(20);
	border_connection(50);
	//Basic Plan

	border_middle1(6);
	printf("Included in plan  %c  Additional usages", 5);
	border_middle2(6);
	border_connection(50);
	//두번째 칸

	border_middle1(1);
	printf("Voice     %c 500m free   %c \\ 50 / 1m", 5, 5);
	border_middle2(14);

	border_middle1(1);
	printf("Text      %c 100t free   %c \\ 10 / 1t", 5, 5);
	border_middle2(14);

	border_middle1(1);
	printf("Data      %c 2.00GB free %c \\ 1000 / 0.1GB", 5, 5);
	border_middle2(9);

	border_connection(50);

	border_middle1(1);
	printf("Basic fee %c \\ 20000", 5);
	border_middle2(30);

	border_bottom(50);
	// 위에 꺼 끝

	//여기부터 밑
	border_top(50);
	border_middle1(18);
	printf("More Data Plan");
	border_middle2(18);
	border_connection(50);


	border_middle1(6);
	printf("Included in plan  %c  Additional usages", 5);
	border_middle2(6);
	border_connection(50);
	//두번째 칸

	border_middle1(1);
	printf("Voice     %c 300m free   %c \\ 10 / 1m", 5, 5);
	border_middle2(14);

	border_middle1(1);
	printf("Text      %c 100t free   %c \\ 30 / 1t", 5, 5);
	border_middle2(14);

	border_middle1(1);
	printf("Data      %c 30.00GB free%c \\ 500 / 0.1GB", 5, 5);
	border_middle2(10);

	border_connection(50);

	border_middle1(1);
	printf("Basic fee %c \\ 45000", 5);
	border_middle2(30);

	border_bottom(50);

	//밑에꺼 (위에꺼 응용)


}

void case2_display(int plan, int voice[], int text[], float data[])
{
	
	system("cls");
	border_top(50);
	border_middle1(14);
	printf("Last two months usage");
	border_middle2(15);
	border_connection(50);
	
	//랜덤:플랜, 통화, 텍스트, 데이터
	


	if (plan == 1)
	{
		border_middle1(1);
		printf("My Plan : Basic Plan");
		border_middle2(29);
		border_connection(50);
	}
	else
	{
		border_middle1(1);
		printf("My Plan : More Data Plan");
		border_middle2(25);
		border_connection(50);
	}
	//1:Basic 2:More Data Plan

	border_middle1(1);
	printf("            %c    Februrary    %c    March    ",5 , 5);
	border_middle2(5);
	border_connection(50);

	border_middle1(1);
	printf("  Voice     %c      %3d min    %c       %3d min  ", 5, voice[0], 5, voice[1]);
	border_middle2(2);

	border_middle1(1);
	printf("  Text      %c      %3d text   %c       %3d text", 5, text[0], 5, text[1]);
	border_middle2(3);

	border_middle1(1);
	printf("  Data      %c   %6.2f GB     %c    %6.2f GB", 5, data[0], 5, data[1]);
	border_middle2(5);

	border_bottom(50);
}

void case3_display(int voice[], int text[], float data[])
{

	system("cls");



	for (int i = 1; i < 36; i++)
	printf("=");
	printf("\n");

	printf("Input your voice : ");
	scanf("%d", &voice[2]);
	printf("\n");

	printf("Input your text : ");
	scanf("%d", &text[2]);
	printf("\n");

	printf("Input your data : ");
	scanf("%f", &data[2]);
}

void case4_display(int plan, int voice[], int text[], float data[], float extra_data[], int fee[])
{
	system("cls");
	extra_data_cal(plan, data, extra_data);
	//extra data 계산중


	//fee 계산

	if (plan == 1)//basic
	{
		for (int i = 0; i < 3; i++)
			fee[i] = 20000;
		for (int i = 0; i < 3; i++)
		{
			fee_cal_basic(voice, text, data, fee, i, extra_data);
		}
	}
	else//more data
	{

		for (int i = 0; i < 3; i++)
			fee[i] = 45000;
		for (int i = 0; i < 3; i++)
		{
			fee_cal_more_data(voice, text, data, fee, i, extra_data);
		}
	}

	border_top(60);
	
	border_middle1(19);
	printf("My three months usage");
	border_middle2(20);

	border_connection(60);

	if (plan == 1)
		{
			border_middle1(1);
			printf("My Plan : Basic Plan");
			border_middle2(39);
			border_connection(60);
		}
	else
		{
			border_middle1(1);
			printf("My Plan : More Data Plan");
			border_middle2(35);
			border_connection(60);
		}

	border_middle1(13);

	border_middle1(3);
	printf("February   ");
	border_middle1(5);
	printf("March      ");
	border_middle1(3);
	printf("April      ");
	border_middle2(0);

	border_connection(60);
	

	border_middle1(1);
	printf("  Voice     %c    %3d min   %c     %3d min    %c    %3d min   ", 5, voice[0], 5, voice[1],5,voice[2]);
	border_middle2(0);

	border_middle1(1);
	printf("  Text      %c    %3d text  %c     %3d text   %c    %3d text  ", 5, text[0], 5, text[1],5, text[2]);
	border_middle2(0);

	border_middle1(1);
	printf("  Data      %c %6.2f GB    %c  %6.2f GB     %c %6.2f GB    ", 5, data[0], 5, data[1],5, data[2]);
	border_middle2(0);

	border_connection(60);

	border_middle1(1);
	
	if (plan == 1)
	{
		printf("Extra Data  %c %6.2f GB    %c  %6.2f GB     %c %6.2f GB    ", 5, extra_data[0], 5, extra_data[1], 5, extra_data[2]); border_middle2(0);
	}
	else
	{
		printf("Extra Data  %c %6.2f GB    %c  %6.2f GB     %c %6.2f GB    ", 5, extra_data[0], 5, extra_data[1], 5, extra_data[2]); border_middle2(0);
	}

	border_connection(60);
	
	border_middle1(1);
	printf("Total Fee   %c \\%10d  %c \\%10d    %c \\%10d", 5, fee[0], 5, fee[1], 5, fee[2]); border_middle2(2);

	border_bottom(60);
}

void case5_display(int plan, int voice[], int text[], float data[])
{
	int average_fee[2] = { 20000, 45000 };

	voice[3] = (voice[0] + voice[1] + voice[2]) / 3;
	text[3] = (text[0] + text[1] + text[2]) / 3;
	data[3] = (data[0] + data[1] + data[2]) / 3;
	
	float tmp1;
	int tmp2;

	tmp1 = data[3] * 100;
	tmp2 = data[3] * 100;
	if (tmp1 - tmp2 >= 0.5)
	{
		data[3] = data[3] - ((((data[3] * 100) - (int)(data[3] * 100))) / 100) + 0.01;
		printf("%f", data[3]);
	}
	else
	{
		data[3] = data[3] - ((((data[3] * 100) - (int)(data[3] * 100))) / 100);
		printf("%f", data[3]);
	}


	if (voice[3] > 500) average_fee[0] += (voice[3] - 500) * 50;
	if (text[3] > 100)  average_fee[0] += (text[3] - 100) * 10;
	if (data[3] > 2.00) average_fee[0] += (data[3] - 2.00) * 10000;


	if (voice[3] > 300) average_fee[1] += (voice[3] - 300) * 10;
	if (text[3] > 100)  average_fee[1] += (text[3] - 100) * 30;
	if (data[3] > 30.00) average_fee[1] += (data[3] - 30.00) * 5000;


	system("cls");

	border_top(50);

	border_middle1(13);
	printf("Average usage of 3-month");
	border_middle2(13);
	
	border_connection(50);
	if (plan == 1)
	{
		border_middle1(1);
		printf("My Plan : Basic Plan");
		border_middle2(29);
		border_connection(50);
	}
	else
	{
		border_middle1(1);
		printf("My Plan : More Data Plan");
		border_middle2(25);
		border_connection(50);
	}

	border_middle1(2);
	printf("Average usage of voice :%8d", voice[3]);
	border_middle2(16);

	border_middle1(2);
	printf("Average usage of text: %9d", text[3]);
	border_middle2(16);

	border_middle1(2);
	printf("Average usage of data: %9.2f", data[3]);
	border_middle2(16);

	border_connection(50);

	border_middle1(2);
	printf("Basic Plan Fee :        \\%9d", average_fee[0]);
	border_middle2(14);


	border_middle1(2);
	printf("More Data Plan Fee :    \\%9d", average_fee[1]);
	border_middle2(14);
	
	border_connection(50);

	if (average_fee[0] >= average_fee[1])
	{
		border_middle1(2);
		printf("We recommend to use More Data Plan");
		border_middle2(14);
	}
	else
	{
		border_middle1(2);
		printf("We recommend to use Basic Plan");
		border_middle2(18);
	}

	border_bottom(50);

}