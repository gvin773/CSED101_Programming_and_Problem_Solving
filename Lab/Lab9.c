#include <stdio.h>

void partial_print(int *arr_start, int size);
int partial_sum(int *arr_start, int *arr_end);

int main()
{
	int a[10];
	int i, start, end, size;
	
	printf("Enter 10 elements of array: ");
	for(i = 0; i < 10; i++)
		scanf("%d", a+i);
		
	printf("Numbers that you enter are: ");
	partial_print(a, 10); //처음부터 size10만큼 
	
	printf("\nEnter the starting and ending index: ");
	scanf("%d%d", &start, &end);
	
	printf("\nSum from array[%d] to array[%d] is %d\n", start, end, partial_sum(a+start, a+end));
	
	size = end - start + 1;
	printf("Partial array output: ");
	partial_print(a+start, size);

	return 0;
}

void partial_print(int *arr_start, int size)
{
	int i;
	
	for(i = 0; i < size; i++)
		printf("%d ", *(arr_start+i));
}

int partial_sum(int *arr_start, int *arr_end)
{
	int *i, sum = 0;
	
	for(i = arr_start; i <= arr_end; i++)
		sum += *i;

	return sum;
}
