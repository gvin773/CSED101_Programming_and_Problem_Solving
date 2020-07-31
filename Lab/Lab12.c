#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

typedef struct
{
	char name[21];
	int exam[2];
	float avg;
	char grade;
} STUDENT;

void add_student(STUDENT slist[], int num);
void print_students(STUDENT slist[], int num);
void find_student(STUDENT slist[], int num);

int main()
{
	STUDENT slist[11];
	char str[100];
	int num = 0;

	while (1)
	{
		printf(">> ");
		scanf("%s", str);

		if (strcmp(str, "add") == 0)
		{
			num++;
			add_student(slist, num);
		}

		else if (strcmp(str, "print") == 0)
			print_students(slist, num);

		else if (strcmp(str, "find") == 0)
			find_student(slist, num);

		else if (strcmp(str, "quit") == 0)
			break;
	}

	return 0;
}

void add_student(STUDENT slist[], int num)
{
		printf("이름: ");
		scanf("%s", slist[num].name);
		printf("중간고사: ");
		scanf("%d", &slist[num].exam[0]);
		printf("기말고사: ");
		scanf("%d", &slist[num].exam[1]);
		printf("\n");

		slist[num].avg = (slist[num].exam[0] + slist[num].exam[1]) / (float)2;

		if (slist[num].avg >= 90) slist[num].grade = 'A';
		else if (slist[num].avg >= 80) slist[num].grade = 'B';
		else if (slist[num].avg >= 70) slist[num].grade = 'C';
		else if (slist[num].avg >= 60) slist[num].grade = 'D';
		else slist[num].grade = 'F';
}

void print_students(STUDENT slist[], int num)
{
	int i;

	printf("번호 이름                 중간 기말  평균 (학점)\n");

	for (i = 1; i <= num; i++)
		printf("%2d   %-20s %2d    %2d   %3.1f   (%c)\n", i, slist[i].name, slist[i].exam[0], slist[i].exam[1], slist[i].avg, slist[i].grade);
	printf("\n");
}

void find_student(STUDENT slist[], int num)
{
	int i;
	char n[21];
	printf("이름: ");
	scanf("%s", n);

	printf("번호 이름                 중간 기말  평균 (학점)\n");

	for (i = 1; i <= num; i++)
	{
		if (strcmp(slist[i].name, n) == 0)
			printf("%2d   %-20s %2d    %2d   %3.1f   (%c)\n", i, slist[i].name, slist[i].exam[0], slist[i].exam[1], slist[i].avg, slist[i].grade);
	}
	printf("\n");
}