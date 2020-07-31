#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#define MAX 6

void bubble_sort(int a[], int size);
void swap(int* x, int* y);
void print_array(int a[], int size);
void input_array(int a[], int size);

int main()
{
    int ary[MAX];

    printf("Input integer numbers to be sorted: ");
    input_array(ary, MAX);

    printf(" Unsorted: ");
    print_array(ary, MAX);

    printf("----------------------------------\n");
    bubble_sort(ary, MAX);
    printf("----------------------------------\n");

    printf("   Sorted: ");
    print_array(ary, MAX);

    return 0;
}

//---------- 함수 정의  ----------//
void bubble_sort(int a[], int size)
{
	int bottom, top;

	for (bottom = 0; bottom < size - 1; bottom++)
	{
		top = size - 1;
		while (1)
		{
			if (top == bottom)
				break;

			if (a[top] < a[top - 1])
				swap(&a[top], &a[top - 1]);

			top -= 1;
		}
		printf("[step%3d]: ", bottom + 1);
		print_array(a, size);
	}
}

void swap(int* x, int* y)
{
	int temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

void print_array(int a[], int size)
{
	int i;

	for (i = 0; i < size; i++)
		printf("%4d", a[i]);

	printf("\n");
}

void input_array(int a[], int size)
{
	int i;

	for (i = 0; i < size; i++)
		scanf("%d", &a[i]);
}