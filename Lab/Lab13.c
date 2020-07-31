#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	char name[21];
	int exam[2];
	float avg;
	char grade;
} STUDENT;

typedef struct node
{
	STUDENT data;
	struct node* next;
} NODE;

typedef struct
{
	int count;
	NODE* head;
	NODE* tail;
} LIST;

void insert_node(LIST* plist);
void add_student(STUDENT* data);
void print_list(LIST* slist);
void find_student(LIST* slist);

int main()
{
	char str[100];
	int num = 0;
	LIST slist;

	slist.count = 0;
	slist.head = '\0';
	slist.tail = '\0';

	while (1)
	{
		printf(">> ");
		scanf("%s", str);

		if (strcmp(str, "add") == 0)
			insert_node(&slist);

		else if (strcmp(str, "print") == 0)
			print_list(&slist);

		else if (strcmp(str, "find") == 0)
			find_student(&slist);

		else if (strcmp(str, "quit") == 0)
			break;
	}

	return 0;
}

void insert_node(LIST* plist)
{
	NODE* pnew;
	pnew = (NODE*)malloc(sizeof(NODE));
	pnew->next = '\0';
	add_student(&pnew->data);

	if (plist->head == '\0')
	{
		plist->head = pnew;
		plist->tail = pnew;
		plist->count++;
	}
	else
	{
		plist->tail->next = pnew;
		plist->tail = pnew;
		plist->count++;
	}
}

void add_student(STUDENT* data)
{
	printf("이름: ");
	scanf("%s", &data->name);
	printf("중간고사: ");
	scanf("%d", &data->exam[0]);
	printf("기말고사: ");
	scanf("%d", &data->exam[1]);
	printf("\n");

	data->avg = (data->exam[0] + data->exam[1]) / (float)2;

	if (data->avg >= 90) data->grade = 'A';
	else if (data->avg >= 80) data->grade = 'B';
	else if (data->avg >= 70) data->grade = 'C';
	else if (data->avg >= 60) data->grade = 'D';
	else data->grade = 'F';
}

void print_list(LIST* slist)
{
	int i = 1;
	NODE* temp;

	printf("번호 이름                 중간 기말  평균 (학점)\n");

	for (temp = slist->head; temp != '\0'; temp = temp->next, i++)
		printf("%2d   %-20s %2d    %2d   %3.1f   (%c)\n", i, temp->data.name, temp->data.exam[0], temp->data.exam[1], temp->data.avg, temp->data.grade);

	printf("\n");
}

void find_student(LIST* slist)
{
	int i = 1;
	NODE* temp;
	char n[21];
	printf("이름: ");
	scanf("%s", n);

	printf("번호 이름                 중간 기말  평균 (학점)\n");

	for (temp = slist->head; temp != '\0'; temp = temp->next, i++)
	{
		if (strcmp(temp->data.name, n) == 0)
			printf("%2d   %-20s %2d    %2d   %3.1f   (%c)\n", i, temp->data.name, temp->data.exam[0], temp->data.exam[1], temp->data.avg, temp->data.grade);
	}
	printf("\n");
}
