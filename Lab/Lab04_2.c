#include <stdio.h>

float calc_average(int a, int b);
char calc_grade(float a);

int main()
{
	int a, b;
	float avg;
	char g;
	
	printf("Enter your midterm score: ");
	scanf("%d", &a);
	
	printf("Enter your final score: ");
	scanf("%d", &b);
	
	avg = calc_average(a, b);
	g = calc_grade(avg);
	
	printf("Average: %.1f\n", avg);
	printf("Grade: %c", g);
	
	return 0;
}

float calc_average(int a, int b)
{
	return (a + b) / (float)2;
}

char calc_grade(float a)
{
	if(a >= 90)
		return 'A';
	
	if(a >= 80)
		return 'B';
	
	if(a >= 70)
		return 'C';
	
	if(a >= 60)
		return 'D';
	
	if(a < 60)
		return 'F';
}
