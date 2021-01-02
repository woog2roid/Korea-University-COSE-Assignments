#include <Stdio.h>

int checkCase(char cha);
void convertCharCase(char* sPtr);

int main(void)
{
	char str[] = "PoINteR is vERy ImPorTAnt in C pRoGraMMing, SO STudEnt nEEd tO pRACtiCe usING poINTer";

	printf("Original String 1 : \n");
	printf("%s\n\n", str);

	convertCharCase(str);

	printf("Original String 2 : \n");
	printf("%s\n\n", str);
}

int checkCase(char cha)
{
	if ('a' <= cha && cha <= 'z')
	{
		return 1;
	}
	if ('A' <= cha && cha <= 'Z')
	{
		return 0;
	}
	else
	{
		return 2;
	}
}

void convertCharCase(char* sPtr)
{
	while (*sPtr != 0)
	{
		if (checkCase(*sPtr)==1)
		{
			*sPtr += ('A' - 'a');
			//소문자
		}
		else if(checkCase(*sPtr)==0)
		{
			*sPtr += ('a' - 'A');
			//대문자
		}
		else
		{
			;
		}
		sPtr++;
	}
}