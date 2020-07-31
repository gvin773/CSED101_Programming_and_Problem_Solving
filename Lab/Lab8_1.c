#include <stdio.h>
#define MAX_SIZE 20

void print_matrix(int a[][MAX_SIZE], int size);

int main()
{
	int a[MAX_SIZE][MAX_SIZE];
	int n, i, j;
	
	printf("Enter the size of square: ");
	scanf("%d", &n);
	
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			if(j < n - i - 1)
				a[i][j] = 1;
			
			else if(j == n - i - 1)
				a[i][j] = 0;
			
			else
				a[i][j] = -1;
		}
	}
	
	print_matrix(a, n);
	
	return 0;
}

void print_matrix(int a[][MAX_SIZE], int size)
{
	int i, j;
	
	for(i = 0; i < size; i++)
	{
		for(j = 0; j < size; j++)
		{
			printf(" %2d", a[i][j]);
		}
		printf("\n");
	}
}
