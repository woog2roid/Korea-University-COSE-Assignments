#include <stdio.h>
#include <ctype.h>
#include <string.h>

//이차원배열을 이용해보자
int main(void)
{
	printf("Enter three lines of text:\n");

	char string[3][80];

	for (int i = 0; i < 3; i++)
	{
		gets(&string[i][0]);
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			string[i][j] = tolower(string[i][j]);
		}
	}

	/*
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			printf("%c", string[i][j]);
		}
	}
	대소문자 변경 완료!!
	*/


	int cnt[26];
	for (int i = 0; i < 26; i++)
	{
		cnt[i] = 0;
	}
	//카운터 0으로 초기화

	char* ptr;

	//int j = 0; j < 3; j++
	//int i = 'a'; i <= 'z'; i++
	for (int j = 0; j < 3; j++)
	{
		for (int i = 'a'; i <= 'z'; i++)
		{
			ptr = strchr(&string[j][0], i);
			while (ptr != NULL)
			{
				cnt[i - 'a']++;
				ptr++;
				ptr = strchr(ptr, i);
			}
		}
	}


	printf("\nThe total occurrences of each character:\n");
	for (int i = 'a'; i <= 'z'; i++)
	{
		printf("%c:%4d\n", i, cnt[i - 'a']);
	}
}