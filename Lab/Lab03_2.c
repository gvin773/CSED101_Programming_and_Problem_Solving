#include <stdio.h>

void divide(int dividend, int divisor, int* quotient, int* remainder);
void print(int quotient, int remainder);

int main()
{
	int a, b, quo, rem;
	
	printf("Enter two integer: ");
	scanf("%d %d", &a, &b);
	
	divide(a, b, &quo, &rem);
	print(quo, rem);
	
	return 0;
}

void divide(int dividend, int divisor, int* quotient, int* remainder)
{
	*quotient=dividend/divisor;
	*remainder=dividend%divisor;
}

void print(int quotient, int remainder)
{
	printf("Quotient :     %d\n", quotient);
	printf("Remainder:     %d", remainder);
}
