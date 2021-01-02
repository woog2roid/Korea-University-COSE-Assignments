#include <stdio.h>

int main(void)
{
	printf("Input 5 integers:");
	int a, b, c, d, e;
	scanf_s("%d%d%d%d%d", &a, &b, &c, &d, &e);
	int A, B, C, D;
	
	if (a >= b) {
		A = a;
	}
	if (a < b) {
		A = b;
	}
	
	if (c >= d) {
		B = c;
	}
	if (c < d) {
		B = d;
	}

	if (A >= B) {
		C = A;
	}
	if (A < B) {
		C = B;
	}

	if (C >= e) {
		D = C;
	}
	if (C < e) {
		D = e;
	}
	printf("The largest value is %d\n", D);

	int A1, B1, C1, D1;
	if (a <= b) {
		A1 = a;
	}
	if (a > b) {
		A1 = b;
	}

	if (c <= d) {
		B1 = c;
	}
	if (c > d) {
		B1 = d;
	}

	if (A1 <= B1) {
		C1 = A1;
	}
	if (A1 > B1) {
		C1 = B1;
	}

	if (C1 <= e) {
		D1 = C1;
	}
	if (C1 > e) {
		D1 = e;
	}
	printf("The smallest value is %d\n", D1);
	
	return 0;
}
