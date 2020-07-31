#include <stdio.h>

int main()
{
    char str[100];
    int i;

    printf("문자열 입력   : ");
    gets(str);

    for(i = 0; str[i] != '\0'; i++)
    {
        if('A' <= str[i] && str[i] <= 'Z')
            str[i] += 32;

        else if('a' <= str[i] && str[i] <= 'z')
            str[i] -= 32;
    }

    printf("변환된 문자열 : %s", str);

    return 0;
}
