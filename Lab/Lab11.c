#include <stdio.h>

int main()
{
    char str[100];
    int i;

    printf("���ڿ� �Է�   : ");
    gets(str);

    for(i = 0; str[i] != '\0'; i++)
    {
        if('A' <= str[i] && str[i] <= 'Z')
            str[i] += 32;

        else if('a' <= str[i] && str[i] <= 'z')
            str[i] -= 32;
    }

    printf("��ȯ�� ���ڿ� : %s", str);

    return 0;
}
