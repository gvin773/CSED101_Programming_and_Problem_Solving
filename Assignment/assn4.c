#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char id[9]; //�й�
	char familyname[31]; //��
	char givenname[31]; //�̸�
	int mid, fin; //�߰�, �⸻
	float avg; //���
	char grade; //����
} STUDENT;

typedef struct node {
	STUDENT stu; //�л�������
	struct node* next; //��忬���
} NODE;

typedef struct {
	NODE* head; //list�� head
	int count; //��尳�� ī���ÿ�
} LIST;

void get_data(LIST *stu_list, FILE *input); //���Ͽ��� ������ �о��
void add_list(LIST *stu_list, const char id[], const char familyname[], const char givenname[], int mid, int fin); //��� ���� �� ����Ʈ�� �߰�
void calc_avggrade(NODE* node); //��հ� ���� ���
void toupper(char command[]); //command�� ���� �빮�ڷ� �ٲ� ��ҹ��� �������� Ŀ�ǵ� �۵�
void print_list(LIST stu_list); //����Ʈ�� ó������ ������ ���
void search_list(LIST stu_list); //�й��� �̿��Ͽ� �л������� �˻��ϰ� ���
void changescore(LIST* stu_list); //����Ʈ�� ����� �л� 1���� ���� ����
void add_student(LIST* stu_list); //����Ʈ�� �л� �߰�
void searchgrade(LIST stu_list); //������ �Է¹޾� �ش��ϴ� �л� ��� ���
void remove_student(LIST* stu_list); //�й��� �Է¹޾� �л��� ����Ʈ���� ����
void quit_program(LIST* stu_list); //�������� txt���Ϸ� ���� �� ���α׷� ����

int main()
{
	FILE *input; //���� ����
	LIST stu_list; //�л� ����Ʈ
	char filename[31]; //���� �̸� ���ڿ�
	char command[31]; //����� Ŀ�ǵ� ���ڿ�

	stu_list.head = NULL;
	stu_list.count = 0;
	//����Ʈ �ʱ�ȭ

	printf("Input the score file name: ");
	scanf("%s", filename);

	input = fopen(filename, "r"); //���� �б� ��� ����

	if (input == NULL) //������ �������� ������
	{
		printf("Could not open %s file\n", filename); //�����޽������
		return 0; //���α׷� ����
	}

	get_data(&stu_list, input); //���Ͽ��� �������а� ����Ʈ ����
	print_list(stu_list); //����Ʈ ���

	while (1)
	{
		printf("\n# ");
		scanf("%s", command); //����Է¹���
		toupper(command); //��ҹ��� ��������

		if (strcmp(command, "SHOW") == 0) //command�� SHOW �Է�
			print_list(stu_list); //����� ����Ʈ ���

		else if (strcmp(command, "SEARCH") == 0)
			search_list(stu_list);

		else if (strcmp(command, "CHANGESCORE") == 0)
			changescore(&stu_list);

		else if (strcmp(command, "ADD") == 0)
			add_student(&stu_list);

		else if (strcmp(command, "SEARCHGRADE") == 0)
			searchgrade(stu_list);

		else if (strcmp(command, "REMOVE") == 0)
			remove_student(&stu_list);

		else if (strcmp(command, "QUIT") == 0)
		{
			quit_program(&stu_list);
			break;
		}

		else //�߸��� �Է�
			printf("WRONG INPUT\n\n");

	}

	fclose(input); //���ϴݱ�
	
	return 0;
}

void get_data(LIST *stu_list, FILE *input) //���Ͽ��� ������ �о��
{
	char id[9]; //�й�
	char familyname[31], givenname[31]; //�̸�
	int mid, fin; //�߰�, �⸻

	while (fscanf(input, "%s%s%s%d%d", id, familyname, givenname, &mid, &fin) != EOF) //���Ͽ��� ������ ����
		add_list(stu_list, id, familyname, givenname, mid, fin); //���������͸� ��忡 ���� �� ����Ʈ�� �߰�
}

void add_list(LIST *stu_list, const char id[], const char familyname[], const char givenname[], int mid, int fin) //��� ���� �� ����Ʈ�� �߰�
{
	NODE* node = malloc(sizeof(NODE)); //��� ����
	NODE* walk; //��� ��ġ ã���� �̿�

	strcpy(node->stu.id, id);
	strcpy(node->stu.familyname, familyname);
	strcpy(node->stu.givenname, givenname);
	node->stu.mid = mid;
	node->stu.fin = fin;
	calc_avggrade(node);
	//������� ���θ��� ��忡 ������ ����

	if (stu_list->count == 0 || stu_list->head->stu.avg <= node->stu.avg) //����Ʈ�� ó���� �����ϴ� ���(ù ��� or �ְ���)
	{
		node->next = stu_list->head; //head�� ���� ����Ǿ� �ִ� node�� ���θ��� node�� ������ ����
		stu_list->head = node; //node�� head�� �����
	}
	
	else //����Ʈ�� �߰� �Ǵ� ���� �����ϴ� ���
	{
		for (walk = stu_list->head; walk->next != NULL; walk = walk->next)
		{
			if (walk->next->stu.avg <= node->stu.avg) //���� ����� ����� ������ �� ū ��ġ�� ã����
				break; //walk ���� ��ġ�� �����ؾ���(��������)
		}

		node->next = walk->next; //node�� ������ walk�� �������� ����
		walk->next = node; //walk�� ������ node�� ����
	}
	//������� ��� ���� �ܰ�

	stu_list->count++; //node�߰������Ƿ� ī��Ʈ +1
}

void calc_avggrade(NODE* node) //��հ� ���� ���
{
	node->stu.avg = (node->stu.mid + node->stu.fin) / (float)2;
	switch ((int)node->stu.avg / 10) { //��տ� ���� ���� ����
	case 10:
	case 9: node->stu.grade = 'A'; break;
	case 8: node->stu.grade = 'B'; break;
	case 7: node->stu.grade = 'C'; break;
	case 6: node->stu.grade = 'D'; break;
	default: node->stu.grade = 'F'; break;
	}
}

void toupper(char command[]) //command�� ���� �빮�ڷ� �ٲ� ��ҹ��� �������� Ŀ�ǵ� �۵�
{
	int i;

	for (i = 0; command[i] != '\0'; i++)
	{
		if (command[i] >= 'a' && command[i] <= 'z')
			command[i] = command[i] - 'a' + 'A';
	}
}

void print_list(LIST stu_list) //����Ʈ�� ó������ ������ ���
{
	NODE* walk;
	char fullname[31];
	
	printf("Student %30s %11s %9s %9s %9s\n", "Name", "Midterm", "Final", "Average", "Grade");
	printf("----------------------------------------------------------------------------------\n");

	for (walk = stu_list.head; walk != NULL; walk = walk->next) //head���� ����Ʈ ���� �˸��� NULL�� ���ö����� ���
	{
		strcpy(fullname, walk->stu.familyname);
		strcat(fullname, " ");
		strcat(fullname, walk->stu.givenname);
		//��, �̸� ��ġ��
		printf("%s%30s%10d%10d%10.1f%10c\n", walk->stu.id, fullname, walk->stu.mid, walk->stu.fin, walk->stu.avg, walk->stu.grade);
	}
}

void search_list(LIST stu_list) //�й��� �̿��Ͽ� �л������� �˻��ϰ� ���
{
	NODE* walk;
	char id[9]; //�˻��� �й�
	char fullname[31];
	
	printf("Student ID: ");
	scanf("%s", id);

	for (walk = stu_list.head; walk != NULL; walk = walk->next) //����Ʈó������ ������ Ž��
	{
		if (strcmp(id, walk->stu.id) == 0) //�˻��� �й��� ���� �й��� ���� �л��� ã����
		{
			strcpy(fullname, walk->stu.familyname);
			strcat(fullname, " ");
			strcat(fullname, walk->stu.givenname);
			//��, �̸� ��ġ��
			printf("\nStudent %30s %11s %9s %9s %9s\n", "Name", "Midterm", "Final", "Average", "Grade");
			printf("----------------------------------------------------------------------------------\n");
			printf("%s%30s%10d%10d%10.1f%10c\n", walk->stu.id, fullname, walk->stu.mid, walk->stu.fin, walk->stu.avg, walk->stu.grade);
			//�л����� ���
			break;
		}
	}

	if(walk == NULL) //��ã����
		printf("NO SUCH PERSON\n");
}

void changescore(LIST* stu_list) //����Ʈ�� ����� �л� 1���� ���� ����
{
	NODE *walk, *walk2;
	char id[9]; //�˻��� �й�
	char test[100]; //�˻��� ����
	int newscore; //�� ����
	char fullname[31];

	printf("Student ID: ");
	scanf("%s", id);
	//�й��Է�

	for (walk = stu_list->head; walk != NULL; walk = walk->next) //����Ʈó������ ������ Ž��
	{
		if (strcmp(id, walk->stu.id) == 0) //�˻��� �й��� ���� �й��� ���� �л��� ã����
			break;
	}

	if (walk == NULL) //��ã����
	{
		printf("NO SUCH PERSON\n");
		return; //�����޽��� ����� �Լ�����
	}

	printf("Mid/Final?: ");
	scanf("%s", test);
	toupper(test);
	//���������Է�

	if (!(strcmp(test, "MID") == 0 || strcmp(test, "FINAL") == 0)) //mid, final�̿��� ���� �ԷµǸ�
	{
		printf("WRONG INPUT\n");
		return; //�����޽��� ����� �Լ�����
	}

	printf("Input new score: ");
	while (1)
	{
		scanf("%d", &newscore);
		if ((newscore >= 0 && newscore <= 100)) //������ �Էµ� ���
			break;

		else //0-100 ���� ���� �Էµ� ���
			printf("INVALID SCORE - Please retype: ");
	}//�������� ���� �Է�

	printf("\nStudent %30s %11s %9s %9s %9s\n", "Name", "Midterm", "Final", "Average", "Grade");
	printf("----------------------------------------------------------------------------------\n");
	strcpy(fullname, walk->stu.familyname);
	strcat(fullname, " ");
	strcat(fullname, walk->stu.givenname);
	printf("%s%30s%10d%10d%10.1f%10c\n", walk->stu.id, fullname, walk->stu.mid, walk->stu.fin, walk->stu.avg, walk->stu.grade);
	//�ٲ�����л��������
	
	if (strcmp(test, "MID") == 0)
		walk->stu.mid = newscore;
	else if (strcmp(test, "FINAL") == 0)
		walk->stu.fin = newscore;
	//�������� ����
	calc_avggrade(walk); //���, ���� ����

	printf("Score changed\n");
	printf("%s%30s%10d%10d%10.1f%10c\n", walk->stu.id, fullname, walk->stu.mid, walk->stu.fin, walk->stu.avg, walk->stu.grade);
	//�ٲ��л��������

	if (walk == stu_list->head) //ù ��尡 �ٲ� ���
		stu_list->head = walk->next;

	else //�߰� �Ǵ� ������ ��尡 �ٲ� ���
	{
		for (walk2 = stu_list->head; walk2->next != walk; walk2 = walk2->next)
			; //walk�� ����� �ּҸ� walk2�� ����

		walk2->next = walk->next;
	}

	if (stu_list->head->stu.avg <= walk->stu.avg) //����Ʈ�� ó������ �ű�� ���(�ְ���)
	{
		walk->next = stu_list->head; //head�� ���� ����Ǿ� �ִ� node�� revised node�� ������ ����
		stu_list->head = walk; //revised node�� head�� �����
	}

	else //����Ʈ�� �߰� �Ǵ� ���� �����ϴ� ���
	{
		for (walk2 = stu_list->head; walk2->next != NULL; walk2 = walk2->next)
		{
			if (walk2->next->stu.avg <= walk->stu.avg) //revised node�� ������ �� ū ��ġ�� ã����
				break; //walk2 ���� ��ġ�� �����ؾ���(��������)
		}

		walk->next = walk2->next; //revised node�� ������ walk2�� �������� ����
		walk2->next = walk; //walk2�� ������ revised node�� ����
	}
	//revised ��� ����
}

void add_student(LIST* stu_list) //����Ʈ�� �л� �߰�
{
	NODE* walk;
	char id[9]; //�й�
	char familyname[31], givenname[31]; //�̸�
	int mid, fin; //�߰�, �⸻

	printf("Student ID: ");
	scanf("%s", id);

	for (walk = stu_list->head; walk != NULL; walk = walk->next) //����Ʈó������ ������ Ž��
	{
		if (strcmp(id, walk->stu.id) == 0) //���� �й��� ���� �л��� �̹� �����ϸ�
		{
			printf("ALREADY EXISTS\n");
			return;
		}
	}
	//������� �й��Է´ܰ�

	printf("Name: ");
	scanf("%s%s", familyname, givenname);
	//������� �̸��Է´ܰ�

	printf("Midterm Score: ");
	while (1)
	{
		scanf("%d", &mid);
		if ((mid >= 0 && mid <= 100)) //������ �Էµ� ���
			break;

		else //0-100 ���� ���� �Էµ� ���
			printf("INVALID SCORE - Please retype: ");
	}//�߰���� ���� �Է�

	printf("Final Score: ");
	while (1)
	{
		scanf("%d", &fin);
		if ((fin >= 0 && fin <= 100)) //������ �Էµ� ���
			break;

		else //0-100 ���� ���� �Էµ� ���
			printf("INVALID SCORE - Please retype: ");
	}//�⸻��� ���� �Է�

	add_list(stu_list, id, familyname, givenname, mid, fin); //�����͸� ��忡 ���� �� ����Ʈ�� �߰�
	printf("Student added\n");
}

void searchgrade(LIST stu_list) //������ �Է¹޾� �ش��ϴ� �л� ��� ���
{
	char grade, fullname[31];
	NODE* walk;
	int count = 0; //�л��� ī����

	printf("Grade to search: ");
	scanf(" %c", &grade); //���۰��๮�ڹ���

	if (!(grade == 'A' || grade == 'B' || grade == 'C' || grade == 'D' || grade == 'F')) //A,B,C,D,F ���� ���� �ԷµǸ�
	{
		printf("WRONG INPUT\n");
		return; //�����޽��� ����� �Լ�����
	}

	printf("Student %30s %11s %9s %9s %9s\n", "Name", "Midterm", "Final", "Average", "Grade");
	printf("----------------------------------------------------------------------------------\n\n");

	for (walk = stu_list.head; walk != NULL; walk = walk->next) //�л�����ƮȮ��
	{
		if (walk->stu.grade == grade) //�Էµ� ������ ������
		{
			strcpy(fullname, walk->stu.familyname);
			strcat(fullname, " ");
			strcat(fullname, walk->stu.givenname);
			printf("%s%30s%10d%10d%10.1f%10c\n", walk->stu.id, fullname, walk->stu.mid, walk->stu.fin, walk->stu.avg, walk->stu.grade);
			//�л��������
			count++; //�л��� ī����
		}
	}

	if (count == 0) //�ش� ������ �л��� ���� ���
		printf("NO RESULTS\n");
}

void remove_student(LIST* stu_list) //�й��� �Է¹޾� �л��� ����Ʈ���� ����
{
	NODE *walk, *temp;
	char id[9], fullname[31], check;
	int count = 0; //�л� ī����

	if (stu_list->count == 0) //����Ʈ�� �ƹ��� ���� ���
	{
		printf("LIST IS EMPTY\n");
		return; //�����޽��� ����� �Լ�����
	}

	printf("Student ID: ");
	scanf("%s", id);

	temp = stu_list->head;
	for (walk = stu_list->head; walk != NULL; walk = walk->next) //�л�����ƮȮ��
	{
		if (strcmp(temp->stu.id, id) == 0) //����Ʈ ù �л��� ����� ���
		{
			count++;
			break;
		}

		if (walk->next != NULL && strcmp(walk->next->stu.id, id) == 0) //�Է¹��� �й� ã����
		{
			count++; //�л� ī����
			temp = walk->next;
			break;
		}
	} //temp�� ���� ��� �ּ� ����

	if (count == 0) //�ش� �й��� �л��� ���� ���
	{
		printf("NO SUCH PERSON\n");
		return;
	}
	
	strcpy(fullname, temp->stu.familyname);
	strcat(fullname, " ");
	strcat(fullname, temp->stu.givenname);
	printf("\nStudent %30s %11s %9s %9s %9s\n", "Name", "Midterm", "Final", "Average", "Grade");
	printf("----------------------------------------------------------------------------------\n");
	printf("%s%30s%10d%10d%10.1f%10c\n", temp->stu.id, fullname, temp->stu.mid, temp->stu.fin, temp->stu.avg, temp->stu.grade);
	//�л����� ���

	printf("\nRemove %s (y/n)? ", temp->stu.id);
	scanf(" %c", &check); //���۰��๮�ڹ���

	if (check == 'y') //y�� �ԷµǸ�
	{
		if (temp == stu_list->head) //ù ��带 �����ϴ� ���
			stu_list->head = temp->next;

		else //�߰� �Ǵ� ������ ��带 �����ϴ� ���
			walk->next = temp->next;

		free(temp); //�޸� �Ҵ� ����(��� ����)
		stu_list->count--;
		printf("Student removed\n"); //����
	}
}

void quit_program(LIST* stu_list) //�������� txt���Ϸ� ���� �� ���α׷� ����
{
	FILE* output;
	NODE *walk;
	char save, filename[31];

	printf("Save data(y/n)? ");
	scanf(" %c", &save); //���۰��๮�ڹ���

	if (save == 'y')
	{
		printf("File name: ");
		scanf("%s", filename);
		output = fopen(filename, "w"); //���ϻ���

		for (walk = stu_list->head; walk != NULL; walk = walk->next)
			fprintf(output, "%s\t%s %s\t%d\t%d\n", walk->stu.id, walk->stu.familyname, walk->stu.givenname, walk->stu.mid, walk->stu.fin);
		//�л����� ���Ͽ� ����
		fclose(output); //���ϴݱ�
	}

	for(walk = stu_list->head; stu_list->head != NULL; walk = stu_list->head)
	{
		stu_list->head = walk->next;
		free(walk);
		stu_list->count--;
	} //����Ʈ���� ��� ��� ����(��� �����Ҵ�� memory free)
}