#include <stdio.h>
#define SIZE 10
int seqSearch(int n[], int i);

int main()
{
	int n[SIZE], i;
	
	printf("Input 10 integer values: ");
	for(i = 0; i < SIZE; i++)
		scanf("%d", &n[i]);
	
	printf("Entered number is      : ");
	for(i = 0; i < SIZE; i++)
		printf("%d ", n[i]);
	
	printf("\nInput an integer that you want to find: ");
	scanf("%d", &i);
	
	if(seqSearch(n,i)==-1)
		printf("Not Found");
	else
		printf("The index of %d is %d", i, seqSearch(n,i));
	
	return 0;
}

int seqSearch(int n[], int i)
{
	int x;
	
	for(x = 0; x < SIZE; x++)
	{
		if(n[x]==i)
			return x;
	}
	
	return -1;
}
