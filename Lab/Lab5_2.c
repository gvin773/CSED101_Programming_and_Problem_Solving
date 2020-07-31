#include <stdio.h>

int main()
{
	int a, r;
	
	printf("Enter a number: ");
	scanf("%d", &a);
	
	while(a != 0)
	{
		printf("%d", a%10);
		a = a/10;
	}
	
	return 0;
}
