#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct listNode
{
	char string[15];
	struct listNode* nextPtr;
};
typedef struct listNode ListNode;
typedef ListNode* ListNodePtr;

void merging(ListNodePtr head1, ListNodePtr head2, ListNodePtr Output);
void Sorting(ListNodePtr Output);

int main(void)
{
	ListNodePtr startPtrs1 = (ListNode*)malloc(sizeof(ListNode));
	(startPtrs1->nextPtr) = NULL;
	ListNodePtr startPtrs2 = (ListNode*)malloc(sizeof(ListNode));
	(startPtrs2->nextPtr) = NULL;

	//단어 읽기
	FILE* fp1, * fp2;
	
	char _str[15];

	ListNodePtr head = (ListNode*)malloc(sizeof(ListNode));

	head = startPtrs1;
	fp1 = fopen("input1.txt", "r");
	for (int cnt = 0; cnt < 10; cnt++)
	{
		for (int i = 0; i < 15; i++)
		{
			_str[i] = 0;
		}
		fscanf(fp1, "%s", _str);

		ListNodePtr newPtr;
		newPtr = (ListNode*)malloc(sizeof(ListNode));

		(newPtr->nextPtr) = (head->nextPtr);
		strcpy(newPtr->string, _str);

		head->nextPtr = newPtr;

		head = newPtr;
	}
	fclose(fp1);

	head = startPtrs2;
	fp2 = fopen("input2.txt", "r");
	for (int cnt = 0; cnt < 10; cnt++)
	{
		for (int i = 0; i < 15; i++)
		{
			_str[i] = 0;
		}

		fscanf(fp2, "%s", _str);

		ListNodePtr newPtr;
		newPtr = (ListNode*)malloc(sizeof(ListNode));

		(newPtr->nextPtr) = (head->nextPtr);
		strcpy(newPtr->string, _str);

		head->nextPtr = newPtr;

		head = newPtr;
	}
	fclose(fp2);
	
	//읽은것 콘솔창에 프린트하기
	ListNodePtr currentPtr = (ListNode*)malloc(sizeof(ListNode));
	
	currentPtr = startPtrs1->nextPtr;
	printf("Input Linked List 1: ");
	printf("%s", currentPtr->string);
	while (currentPtr != NULL)
	{
		currentPtr = currentPtr->nextPtr;
		if (currentPtr == NULL)
		{
			break;
		}
		printf(" -> %s", currentPtr->string);
	}
	printf("\n");
	printf("\n");

	currentPtr = startPtrs2->nextPtr;
	printf("Input Linked List 2: ");
	printf("%s", currentPtr->string);
	while (currentPtr != NULL)
	{
		currentPtr = currentPtr->nextPtr;
		if (currentPtr == NULL)
		{
			break;
		}
		printf(" -> %s", currentPtr->string);
	}
	printf("\n");
	printf("\n");
	
	ListNodePtr Output = (ListNodePtr)malloc(sizeof(ListNode));
	(Output->nextPtr) = NULL;
	//여기까진 잘됨

	merging(startPtrs1, startPtrs2, Output);
	//두 linked list 합성

	Sorting(Output);

	//프린트 하는거 문제 없어보임
	printf("Result linked list: ");
	currentPtr = Output->nextPtr;
	printf("%s", currentPtr->string);
	while (currentPtr != NULL)
	{
		currentPtr = currentPtr->nextPtr;
		if (currentPtr == NULL)
		{
			break;
		}
		printf(" -> %s", currentPtr->string);
	}

	FILE* fpw = fopen("output.txt", "w");
	currentPtr = Output->nextPtr;
	fprintf(fpw, "%s\n", currentPtr->string);
	while (currentPtr != NULL)
	{
		currentPtr = currentPtr->nextPtr;
		if (currentPtr == NULL)
		{
			break;
		}
		fprintf(fpw, "%s\n", currentPtr->string);
	}
	fclose(fpw);

	return 0;
}

void merging(ListNodePtr head1, ListNodePtr head2, ListNodePtr Output)
{
	for (int i = 0; i < 10; i++)
	{
		ListNodePtr newnode = (ListNodePtr)malloc((sizeof(ListNode)));

		head1 = (head1->nextPtr);//head 이동

		strcpy((newnode -> string) , (head1 -> string));//뉴노드 스트링 저장

		newnode->nextPtr = Output->nextPtr;//Ptr연결
		Output->nextPtr = newnode;
	}

	for (int i = 0; i < 10; i++)
	{
		ListNodePtr newnode = (ListNodePtr)malloc((sizeof(ListNode)));

		head2 = (head2->nextPtr);//head 이동
		strcpy((newnode->string), (head2->string));//뉴노드 스트링 저장

		newnode->nextPtr = Output->nextPtr;
		Output->nextPtr = newnode;
	}
	
}

void Sorting(ListNodePtr Output)
{
	
	for (int i = 0; i < 20; i++)
	{
		ListNodePtr p1 = Output->nextPtr;
		while (p1->nextPtr != NULL)
		{
			if (strcmp((p1->string), (p1->nextPtr->string)) > 0)
			{
				char tmp[15];
				strcpy(tmp, p1->string);
				strcpy(p1->string, p1->nextPtr->string);
				strcpy(p1->nextPtr->string, tmp);
				//순서 바꾸기
			}
			p1 = p1->nextPtr;
		}
	}
}