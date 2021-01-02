#include <stdio.h>

void function(int* a, int* b, int* c);

int main(void)
{
	printf("Enter three numbers:");
	int a, b, c;
	scanf("%d%d%d", &a, &b, &c);
	int* aptr, * bptr, * cptr;
	
	aptr = &a;
	bptr = &b;
	cptr = &c;
	
	printf("\nBefore: a = %d, b= %d, c = %d\n", *aptr, *bptr, *cptr);
	function(aptr, bptr, cptr);
	printf("\nAfter: a = %d, b= %d, c = %d\n", *aptr, *bptr, *cptr);
}

void function(int* a, int* b, int* c)
{
	*a += 1;
	*b *= *c;
	int tmp;
	tmp = *b;
	*b = *c;
	*c = tmp;
}