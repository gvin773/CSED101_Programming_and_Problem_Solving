#include <stdio.h>

int add(int a, int b);
float average(int a, int b);
void printResults(int x, int y, int sum, float avg);

int main()
{
	int a, b;
	
	printf("Enter two integers: ");
	scanf("%d %d", &a, &b);
	
	printResults(a, b, add(a, b), average(a, b));
	
	return 0;
}

int add(int a, int b)
{
	return a+b;
}

float average(int a, int b)
{
	return (float)(a + b)/2;
}

void printResults(int x, int y, int sum, float avg)
{
	printf("The sum of %d and %d is %d\n", x, y, sum);
	printf("The average of numbers is %.1f", avg);
}
