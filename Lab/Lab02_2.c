#include <stdio.h>

int main()
{
	double r;
	
	printf("반지름을 입력하시오: ");
	scanf("%lf", &r);
	
	printf("원 둘레: %lf\n", 2*3.141592*r);
	printf("원 넓이: %lf", 3.141592*r*r);
	
	return 0;
}
