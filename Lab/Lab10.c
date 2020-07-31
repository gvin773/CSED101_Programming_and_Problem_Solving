#include <stdio.h>
#include <stdlib.h>

void find_minmax(int *arr, int size, int *max, int *min);

int main()
{
    int n, i;
    int *arr;
    int min, max;

    printf("Enter array size: ");
    scanf("%d", &n);

    arr = malloc(sizeof(int) * n);

    for(i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    printf("Entered numbers are\n");
    for(i = 0; i < n; i++)
        printf("%d ", arr[i]);

    find_minmax(arr, n, &max, &min);

    printf("\nMaximum is %d\n", max);
    printf("Minimum is %d\n", min);

    printf("\nEnter changed array size: ");
    scanf("%d", &n);

    arr = realloc(arr, sizeof(int) * n);

    for(i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    printf("Entered numbers are\n");
    for(i = 0; i <n; i++)
        printf("%d ", arr[i]);

    find_minmax(arr, n, &max, &min);

    printf("\nMaximum is %d\n", max);
    printf("Minimum is %d", min);

    free(arr);

    return 0;
}

void find_minmax(int *arr, int size, int *max, int *min)
{
    int i;

    *max = arr[0];
    *min = arr[0];

    for(i = 1; i < size; i++)
    {
        if(*max < arr[i])
            *max = arr[i];

        if(*min > arr[i])
            *min = arr[i];
    }
}
