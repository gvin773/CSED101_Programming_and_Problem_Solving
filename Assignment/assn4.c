#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char id[9]; //학번
	char familyname[31]; //성
	char givenname[31]; //이름
	int mid, fin; //중간, 기말
	float avg; //평균
	char grade; //평점
} STUDENT;

typedef struct node {
	STUDENT stu; //학생데이터
	struct node* next; //노드연결용
} NODE;

typedef struct {
	NODE* head; //list의 head
	int count; //노드개수 카운팅용
} LIST;

void get_data(LIST *stu_list, FILE *input); //파일에서 데이터 읽어옴
void add_list(LIST *stu_list, const char id[], const char familyname[], const char givenname[], int mid, int fin); //노드 생성 후 리스트에 추가
void calc_avggrade(NODE* node); //평균과 평점 계산
void toupper(char command[]); //command를 전부 대문자로 바꿔 대소문자 구별없이 커맨드 작동
void print_list(LIST stu_list); //리스트를 처음부터 끝까지 출력
void search_list(LIST stu_list); //학번을 이용하여 학생정보를 검색하고 출력
void changescore(LIST* stu_list); //리스트에 저장된 학생 1명의 점수 수정
void add_student(LIST* stu_list); //리스트에 학생 추가
void searchgrade(LIST stu_list); //평점을 입력받아 해당하는 학생 모두 출력
void remove_student(LIST* stu_list); //학번을 입력받아 학생을 리스트에서 제거
void quit_program(LIST* stu_list); //편집내용 txt파일로 저장 및 프로그램 종료

int main()
{
	FILE *input; //파일 연결
	LIST stu_list; //학생 리스트
	char filename[31]; //파일 이름 문자열
	char command[31]; //사용자 커맨드 문자열

	stu_list.head = NULL;
	stu_list.count = 0;
	//리스트 초기화

	printf("Input the score file name: ");
	scanf("%s", filename);

	input = fopen(filename, "r"); //파일 읽기 모드 연결

	if (input == NULL) //파일이 존재하지 않으면
	{
		printf("Could not open %s file\n", filename); //에러메시지출력
		return 0; //프로그램 종료
	}

	get_data(&stu_list, input); //파일에서 데이터읽고 리스트 만듦
	print_list(stu_list); //리스트 출력

	while (1)
	{
		printf("\n# ");
		scanf("%s", command); //명령입력받음
		toupper(command); //대소문자 구별가능

		if (strcmp(command, "SHOW") == 0) //command로 SHOW 입력
			print_list(stu_list); //저장된 리스트 출력

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

		else //잘못된 입력
			printf("WRONG INPUT\n\n");

	}

	fclose(input); //파일닫기
	
	return 0;
}

void get_data(LIST *stu_list, FILE *input) //파일에서 데이터 읽어옴
{
	char id[9]; //학번
	char familyname[31], givenname[31]; //이름
	int mid, fin; //중간, 기말

	while (fscanf(input, "%s%s%s%d%d", id, familyname, givenname, &mid, &fin) != EOF) //파일에서 데이터 읽음
		add_list(stu_list, id, familyname, givenname, mid, fin); //읽은데이터를 노드에 저장 후 리스트에 추가
}

void add_list(LIST *stu_list, const char id[], const char familyname[], const char givenname[], int mid, int fin) //노드 생성 후 리스트에 추가
{
	NODE* node = malloc(sizeof(NODE)); //노드 생성
	NODE* walk; //노드 위치 찾을때 이용

	strcpy(node->stu.id, id);
	strcpy(node->stu.familyname, familyname);
	strcpy(node->stu.givenname, givenname);
	node->stu.mid = mid;
	node->stu.fin = fin;
	calc_avggrade(node);
	//여기까지 새로만든 노드에 데이터 저장

	if (stu_list->count == 0 || stu_list->head->stu.avg <= node->stu.avg) //리스트의 처음에 삽입하는 경우(첫 노드 or 최고점)
	{
		node->next = stu_list->head; //head에 원래 연결되어 있던 node를 새로만든 node의 다음에 연결
		stu_list->head = node; //node가 head에 연결됨
	}
	
	else //리스트의 중간 또는 끝에 삽입하는 경우
	{
		for (walk = stu_list->head; walk->next != NULL; walk = walk->next)
		{
			if (walk->next->stu.avg <= node->stu.avg) //새로 저장된 노드의 점수가 더 큰 위치를 찾으면
				break; //walk 다음 위치에 저장해야함(내림차순)
		}

		node->next = walk->next; //node의 다음을 walk의 다음으로 연결
		walk->next = node; //walk의 다음을 node로 변경
	}
	//여기까지 노드 삽입 단계

	stu_list->count++; //node추가했으므로 카운트 +1
}

void calc_avggrade(NODE* node) //평균과 평점 계산
{
	node->stu.avg = (node->stu.mid + node->stu.fin) / (float)2;
	switch ((int)node->stu.avg / 10) { //평균에 따라 평점 저장
	case 10:
	case 9: node->stu.grade = 'A'; break;
	case 8: node->stu.grade = 'B'; break;
	case 7: node->stu.grade = 'C'; break;
	case 6: node->stu.grade = 'D'; break;
	default: node->stu.grade = 'F'; break;
	}
}

void toupper(char command[]) //command를 전부 대문자로 바꿔 대소문자 구별없이 커맨드 작동
{
	int i;

	for (i = 0; command[i] != '\0'; i++)
	{
		if (command[i] >= 'a' && command[i] <= 'z')
			command[i] = command[i] - 'a' + 'A';
	}
}

void print_list(LIST stu_list) //리스트를 처음부터 끝까지 출력
{
	NODE* walk;
	char fullname[31];
	
	printf("Student %30s %11s %9s %9s %9s\n", "Name", "Midterm", "Final", "Average", "Grade");
	printf("----------------------------------------------------------------------------------\n");

	for (walk = stu_list.head; walk != NULL; walk = walk->next) //head부터 리스트 끝을 알리는 NULL이 나올때까지 출력
	{
		strcpy(fullname, walk->stu.familyname);
		strcat(fullname, " ");
		strcat(fullname, walk->stu.givenname);
		//성, 이름 합치기
		printf("%s%30s%10d%10d%10.1f%10c\n", walk->stu.id, fullname, walk->stu.mid, walk->stu.fin, walk->stu.avg, walk->stu.grade);
	}
}

void search_list(LIST stu_list) //학번을 이용하여 학생정보를 검색하고 출력
{
	NODE* walk;
	char id[9]; //검색할 학번
	char fullname[31];
	
	printf("Student ID: ");
	scanf("%s", id);

	for (walk = stu_list.head; walk != NULL; walk = walk->next) //리스트처음부터 끝까지 탐색
	{
		if (strcmp(id, walk->stu.id) == 0) //검색할 학번과 같은 학번을 가진 학생을 찾으면
		{
			strcpy(fullname, walk->stu.familyname);
			strcat(fullname, " ");
			strcat(fullname, walk->stu.givenname);
			//성, 이름 합치기
			printf("\nStudent %30s %11s %9s %9s %9s\n", "Name", "Midterm", "Final", "Average", "Grade");
			printf("----------------------------------------------------------------------------------\n");
			printf("%s%30s%10d%10d%10.1f%10c\n", walk->stu.id, fullname, walk->stu.mid, walk->stu.fin, walk->stu.avg, walk->stu.grade);
			//학생정보 출력
			break;
		}
	}

	if(walk == NULL) //못찾으면
		printf("NO SUCH PERSON\n");
}

void changescore(LIST* stu_list) //리스트에 저장된 학생 1명의 점수 수정
{
	NODE *walk, *walk2;
	char id[9]; //검색할 학번
	char test[100]; //검색할 시험
	int newscore; //새 점수
	char fullname[31];

	printf("Student ID: ");
	scanf("%s", id);
	//학번입력

	for (walk = stu_list->head; walk != NULL; walk = walk->next) //리스트처음부터 끝까지 탐색
	{
		if (strcmp(id, walk->stu.id) == 0) //검색할 학번과 같은 학번을 가진 학생을 찾으면
			break;
	}

	if (walk == NULL) //못찾으면
	{
		printf("NO SUCH PERSON\n");
		return; //에러메시지 출력후 함수종료
	}

	printf("Mid/Final?: ");
	scanf("%s", test);
	toupper(test);
	//시험종류입력

	if (!(strcmp(test, "MID") == 0 || strcmp(test, "FINAL") == 0)) //mid, final이외의 값이 입력되면
	{
		printf("WRONG INPUT\n");
		return; //에러메시지 출력후 함수종료
	}

	printf("Input new score: ");
	while (1)
	{
		scanf("%d", &newscore);
		if ((newscore >= 0 && newscore <= 100)) //정상값이 입력된 경우
			break;

		else //0-100 외의 값이 입력된 경우
			printf("INVALID SCORE - Please retype: ");
	}//수정받을 점수 입력

	printf("\nStudent %30s %11s %9s %9s %9s\n", "Name", "Midterm", "Final", "Average", "Grade");
	printf("----------------------------------------------------------------------------------\n");
	strcpy(fullname, walk->stu.familyname);
	strcat(fullname, " ");
	strcat(fullname, walk->stu.givenname);
	printf("%s%30s%10d%10d%10.1f%10c\n", walk->stu.id, fullname, walk->stu.mid, walk->stu.fin, walk->stu.avg, walk->stu.grade);
	//바뀌기전학생정보출력
	
	if (strcmp(test, "MID") == 0)
		walk->stu.mid = newscore;
	else if (strcmp(test, "FINAL") == 0)
		walk->stu.fin = newscore;
	//새점수로 수정
	calc_avggrade(walk); //평균, 평점 수정

	printf("Score changed\n");
	printf("%s%30s%10d%10d%10.1f%10c\n", walk->stu.id, fullname, walk->stu.mid, walk->stu.fin, walk->stu.avg, walk->stu.grade);
	//바뀐학생정보출력

	if (walk == stu_list->head) //첫 노드가 바뀐 경우
		stu_list->head = walk->next;

	else //중간 또는 마지막 노드가 바뀐 경우
	{
		for (walk2 = stu_list->head; walk2->next != walk; walk2 = walk2->next)
			; //walk앞 노드의 주소를 walk2에 저장

		walk2->next = walk->next;
	}

	if (stu_list->head->stu.avg <= walk->stu.avg) //리스트의 처음으로 옮기는 경우(최고점)
	{
		walk->next = stu_list->head; //head에 원래 연결되어 있던 node를 revised node의 다음에 연결
		stu_list->head = walk; //revised node가 head에 연결됨
	}

	else //리스트의 중간 또는 끝에 삽입하는 경우
	{
		for (walk2 = stu_list->head; walk2->next != NULL; walk2 = walk2->next)
		{
			if (walk2->next->stu.avg <= walk->stu.avg) //revised node의 점수가 더 큰 위치를 찾으면
				break; //walk2 다음 위치에 저장해야함(내림차순)
		}

		walk->next = walk2->next; //revised node의 다음을 walk2의 다음으로 연결
		walk2->next = walk; //walk2의 다음을 revised node로 변경
	}
	//revised 노드 삽입
}

void add_student(LIST* stu_list) //리스트에 학생 추가
{
	NODE* walk;
	char id[9]; //학번
	char familyname[31], givenname[31]; //이름
	int mid, fin; //중간, 기말

	printf("Student ID: ");
	scanf("%s", id);

	for (walk = stu_list->head; walk != NULL; walk = walk->next) //리스트처음부터 끝까지 탐색
	{
		if (strcmp(id, walk->stu.id) == 0) //같은 학번을 가진 학생이 이미 존재하면
		{
			printf("ALREADY EXISTS\n");
			return;
		}
	}
	//여기까지 학번입력단계

	printf("Name: ");
	scanf("%s%s", familyname, givenname);
	//여기까지 이름입력단계

	printf("Midterm Score: ");
	while (1)
	{
		scanf("%d", &mid);
		if ((mid >= 0 && mid <= 100)) //정상값이 입력된 경우
			break;

		else //0-100 외의 값이 입력된 경우
			printf("INVALID SCORE - Please retype: ");
	}//중간고사 점수 입력

	printf("Final Score: ");
	while (1)
	{
		scanf("%d", &fin);
		if ((fin >= 0 && fin <= 100)) //정상값이 입력된 경우
			break;

		else //0-100 외의 값이 입력된 경우
			printf("INVALID SCORE - Please retype: ");
	}//기말고사 점수 입력

	add_list(stu_list, id, familyname, givenname, mid, fin); //데이터를 노드에 저장 후 리스트에 추가
	printf("Student added\n");
}

void searchgrade(LIST stu_list) //평점을 입력받아 해당하는 학생 모두 출력
{
	char grade, fullname[31];
	NODE* walk;
	int count = 0; //학생수 카운팅

	printf("Grade to search: ");
	scanf(" %c", &grade); //버퍼개행문자무시

	if (!(grade == 'A' || grade == 'B' || grade == 'C' || grade == 'D' || grade == 'F')) //A,B,C,D,F 외의 값이 입력되면
	{
		printf("WRONG INPUT\n");
		return; //에러메시지 출력후 함수종료
	}

	printf("Student %30s %11s %9s %9s %9s\n", "Name", "Midterm", "Final", "Average", "Grade");
	printf("----------------------------------------------------------------------------------\n\n");

	for (walk = stu_list.head; walk != NULL; walk = walk->next) //학생리스트확인
	{
		if (walk->stu.grade == grade) //입력된 평점과 같으면
		{
			strcpy(fullname, walk->stu.familyname);
			strcat(fullname, " ");
			strcat(fullname, walk->stu.givenname);
			printf("%s%30s%10d%10d%10.1f%10c\n", walk->stu.id, fullname, walk->stu.mid, walk->stu.fin, walk->stu.avg, walk->stu.grade);
			//학생정보출력
			count++; //학생수 카운팅
		}
	}

	if (count == 0) //해당 평점의 학생이 없는 경우
		printf("NO RESULTS\n");
}

void remove_student(LIST* stu_list) //학번을 입력받아 학생을 리스트에서 제거
{
	NODE *walk, *temp;
	char id[9], fullname[31], check;
	int count = 0; //학생 카운팅

	if (stu_list->count == 0) //리스트에 아무도 없을 경우
	{
		printf("LIST IS EMPTY\n");
		return; //에러메시지 출력후 함수종료
	}

	printf("Student ID: ");
	scanf("%s", id);

	temp = stu_list->head;
	for (walk = stu_list->head; walk != NULL; walk = walk->next) //학생리스트확인
	{
		if (strcmp(temp->stu.id, id) == 0) //리스트 첫 학생을 지우는 경우
		{
			count++;
			break;
		}

		if (walk->next != NULL && strcmp(walk->next->stu.id, id) == 0) //입력받은 학번 찾으면
		{
			count++; //학생 카운팅
			temp = walk->next;
			break;
		}
	} //temp에 지울 노드 주소 저장

	if (count == 0) //해당 학번의 학생이 없는 경우
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
	//학생정보 출력

	printf("\nRemove %s (y/n)? ", temp->stu.id);
	scanf(" %c", &check); //버퍼개행문자무시

	if (check == 'y') //y가 입력되면
	{
		if (temp == stu_list->head) //첫 노드를 삭제하는 경우
			stu_list->head = temp->next;

		else //중간 또는 마지막 노드를 삭제하는 경우
			walk->next = temp->next;

		free(temp); //메모리 할당 해제(노드 제거)
		stu_list->count--;
		printf("Student removed\n"); //삭제
	}
}

void quit_program(LIST* stu_list) //편집내용 txt파일로 저장 및 프로그램 종료
{
	FILE* output;
	NODE *walk;
	char save, filename[31];

	printf("Save data(y/n)? ");
	scanf(" %c", &save); //버퍼개행문자무시

	if (save == 'y')
	{
		printf("File name: ");
		scanf("%s", filename);
		output = fopen(filename, "w"); //파일생성

		for (walk = stu_list->head; walk != NULL; walk = walk->next)
			fprintf(output, "%s\t%s %s\t%d\t%d\n", walk->stu.id, walk->stu.familyname, walk->stu.givenname, walk->stu.mid, walk->stu.fin);
		//학생정보 파일에 저장
		fclose(output); //파일닫기
	}

	for(walk = stu_list->head; stu_list->head != NULL; walk = stu_list->head)
	{
		stu_list->head = walk->next;
		free(walk);
		stu_list->count--;
	} //리스트에서 모든 노드 제거(모든 동적할당된 memory free)
}