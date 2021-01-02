#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct queueNode
{
	char data;
	struct queueNode* nextPtr;
};
typedef struct queueNode QueueNode;
typedef QueueNode* QueueNodePtr;

void printQueue(QueueNodePtr currentPtr);
int isEmpty(QueueNodePtr headPtr);
char dequeue(QueueNodePtr* headPtr, QueueNodePtr* tailPtr);
void enqueue(QueueNodePtr* headPtr, QueueNodePtr* tailPtr, char value);
void instructions();

int main()
{
	QueueNodePtr headPtr = NULL;
	QueueNodePtr tailPtr = NULL;
	int choice;
	char item;

	instructions();
	printf("? ");
	scanf("%d", &choice);

	while (choice != 3)
	{
		switch (choice)
		{
			case 1:
				while (!getchar());
				printf("Enter a character: ");
				item = getchar();
				enqueue(&headPtr, &tailPtr, item);
				printQueue(headPtr);
				break;
			
			case 2:
				while (!getchar());
				if (!isEmpty(headPtr))
				{
					item = dequeue(&headPtr, &tailPtr);
					printf("%c has been dequeued \n", item);
				}
				printQueue(headPtr);
				break;

			default:
				while (!getchar());
				printf("Invalid choice. \n\n");
				instructions();
				break;
		}
		printf("? ");
		scanf("%d", &choice);
	}
	printf("End of run. \n");
	return 0;
}

void instructions()
{
	printf("Enter your choice: \n");
	printf("\t1 to add an item to the queue \n");
	printf("\t2 to remove an item from the queue \n");
	printf("\t3 to end \n");
}

int isEmpty(QueueNodePtr headPtr)
{
	return headPtr == NULL;
}

void printQueue(QueueNodePtr currentPtr)
{
	if (isEmpty(currentPtr))
	{
		printf("Queue is empty\n\n");
	}
	else
	{
		printf("The queue is : \n");

		while (currentPtr != NULL)
		{
			printf("%c --> ", currentPtr->data);
			currentPtr = currentPtr->nextPtr;
		}
		printf("NULL \n\n");
	}
}

char dequeue(QueueNodePtr* headPtr, QueueNodePtr* tailPtr)
{
	char value;
	QueueNodePtr tempPtr;
		
	value = (*headPtr)->data;
	tempPtr = *headPtr;
	*headPtr = (*headPtr)->nextPtr;
	if (*headPtr == NULL)
	{
		*tailPtr = NULL;
	}
}
void enqueue(QueueNodePtr* headPtr, QueueNodePtr* tailPtr, char value)
{
	QueueNodePtr newPtr;
	newPtr = malloc(sizeof(QueueNode));

	if (newPtr != NULL)
	{
		newPtr->data = value;
		newPtr->nextPtr = NULL;

		if (isEmpty(*headPtr))
		{
			*headPtr = newPtr;
		}

		else
		{
			(*tailPtr)->nextPtr = newPtr;
		}

		*tailPtr = newPtr;
	}
	else
	{
		printf("%c not inserted. No memory available.\n", value);
	}
}