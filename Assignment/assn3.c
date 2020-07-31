#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <math.h>

void input(int* row, int* col); //행렬의 크기를 받는 함수
void print_board(int row, int col, int** board, int score[], int count_turn, int** cursor1, int** cursor2); //보드판 출력 함수
void turn(int** board, int** cursor1, int** cursor2, int* count_turn, int* x1, int* y1, int* x2, int* y2, int row, int col, int* count_choice, int score[]); //플레이어의 턴을 진행해주는 함수
void gotoxy(int x, int y); //커서 이동 함수
void space(int** board, int x, int y, int count_turn, int* count_choice); //말 선택 함수
int test(int** board, int x, int y, int count_turn, int x0, int y0, float* d); //적절한 선택인지 테스트하는 함수
int test_distance(int x0, int y0, int x, int y, float* d); //거리 체크 함수
int test_move(int** board, int* count_turn, int row, int col, int score[]); //플레이어의 말이 움직일 곳이 있는지 테스트하는 함수

int main()
{
	int row, col, i, j, count_turn, count_choice, break_count = 0, len;
	int x1, y1, x2, y2;
	int** board, **cursor1, **cursor2;
	int score[3] = { 0, 2, 2 }; //1은 플레이어1, 2는 플레이어 2
	char re[10000];

	while (1)
	{
		count_turn = 1, count_choice = 0, x1 = 0, y1 = 0, score[1] = 2, score[2] = 2; //게임초기값 세팅
		system("cls");
		printf("[ATAXX]\n");
		input(&row, &col); //행렬 크기 입력받음

		system("cls");

		board = (int**)calloc(row, sizeof(int*));
		for (i = 0; i < row; i++)
			*(board + i) = (int*)calloc(col, sizeof(int));
		//보드판 동적할당, 0은 주인없는 구역, 1은 1의 구역, 2는 2의 구역, 3은 선택된 말

		cursor1 = (int**)calloc(row, sizeof(int*));
		for (i = 0; i < row; i++)
			*(cursor1 + i) = (int*)calloc(col, sizeof(int));
		//커서1 동적할당, 커서1이 있는 곳은 1 없는 곳은 0

		cursor2 = (int**)calloc(row, sizeof(int*));
		for (i = 0; i < row; i++)
			*(cursor2 + i) = (int*)calloc(col, sizeof(int));
		//커서2 동적할당, 커서2가 있는 곳은 1 없는 곳은 0

		//게임준비단계
		for (i = 0; i < row; i++) //보드, 커서1, 커서2 초기화
			for (j = 0; j < col; j++)
			{
				board[i][j] = 0;
				cursor1[i][j] = 0;
				cursor2[i][j] = 0;
			}

		board[0][0] = 1, board[0][col - 1] = 1, cursor1[0][0] = 1; //플레이어1 초기 세팅
		board[row - 1][0] = 2, board[row - 1][col - 1] = 2, cursor2[row - 1][col - 1] = 1; //플레이어2 초기 세팅
		x2 = (col - 1), y2 = (row - 1);

		//게임시작
		print_board(row, col, board, score, count_turn, cursor1, cursor2);
		gotoxy(x1 * 4, y1 * 2);
		while (1)
		{
			turn(board, cursor1, cursor2, &count_turn, &x1, &y1, &x2, &y2, row, col, &count_choice, score);
			system("cls");
			print_board(row, col, board, score, count_turn, cursor1, cursor2);

			if (score[1] + score[2] == row * col || score[1] == 0 || score[2] == 0) //말이 가득찬 경우 또는 어느 한쪽 플레이어의 말만 남은 경우
				break;

			if (test_move(board, &count_turn, row, col, score)) //턴이 돌아온 플레이어가 움직일 말이 없다면 다른 플레이어의 턴으로 바꿔줌
			{
				system("cls");
				print_board(row, col, board, score, count_turn, cursor1, cursor2);
			}

			if (count_turn == 1) //플레이어 1의 차례이면
				gotoxy(x1 * 4, y1 * 2);
			else //플레이어 2의 차례이면
				gotoxy(x2 * 4, y2 * 2);
		}

		gotoxy(-2, 2 * row);
		printf("\n");
		if (score[1] == score[2]) //무승부
			printf("Draw\n");
		else if (score[1] > score[2]) //플레이어 1의 승리
			printf("Player 1 wins\n");

		else //플레이어 2의 승리
			printf("Player 2 wins\n");

		for (i = 0; i < row; i++)
			free(board[i]);
		free(board);

		for (i = 0; i < row; i++)
			free(cursor1[i]);
		free(cursor1);

		for (i = 0; i < row; i++)
			free(cursor2[i]);
		free(cursor2); //게임종료 동적할당 해제

		while (1)
		{
			len = 0;
			printf("Continue? (y/n)");
			scanf("%s", &re);

			for (i = 0; re[i] != '\0'; i++) //문자열 끝까지
				len += 1; //문자열 길이 저장

			if (len > 1) //2글자 이상을 입력한 경우
				printf("y 또는 n을 입력해주세요\n");

			else //1글자를 입력한 경우
			{
				if (re[0] != 'n' && re[0] != 'y') //y또는 n이 입력되지 않은 경우
					printf("y또는 n을 입력해주세요\n");

				if (re[0] == 'n') //게임재시작x
				{
					break_count = 1;
					break;
				}

				else if (re[0] == 'y') //게임 재시작 o
					break;
			}
		}

		if (break_count) //n이 입력된경우 프로그램 종료
			break;
	}

	return 0;
}

void input(int* row, int* col)
{
	int count, i, j;
	char str[2][1000];

	while (1)
	{
		count = 0;
		printf("row: ");
		scanf("%s", str[0]);
		printf("col: ");
		scanf("%s", str[1]);

		for (i = 0; i < 2; i++)
		{
			for (j = 0; str[i][j] != '\0'; j++)
			{
				if (str[i][j] < '0' || str[i][j] > '9') //문자가 있으면
				{
					count = 1; //에러카운트
					break; //더볼필요없음
				}
			}
		}

		if(count != 1) //숫자로만 이루어진 문자열 확정
		{
			for (i = 0; i < 2; i++)
			{
				if (str[i][1] == '\0') //1자리수
				{
					if (str[i][0] < '4')
					{
						count = 1; //4미만의 수가 입력된경우
						break; //다른 문자열은 확인할 필요없음
					}

					else //4이상의 한자리수, 정상케이스
					{
						if (i == 0) *row = str[i][0] - '0'; //row에 값대입
						else *col = str[i][0] - '0'; //col에 값대입
						count = 100; //무한반복 탈출
					}
				}

				else if (str[i][2] == '\0') //2자리수
				{
					if (str[i][0] == '0')
					{
						count = 2; //01, 02 이런식의 입력인경우
						break; //다른 문자열은 확인할 필요없음
					}

					else if (str[i][0] == '1') //1ㅁ의 형태, 정상케이스
					{
						if (i == 0) *row = 10 + str[i][1] - '0';
						else *col = 10 + str[i][1] - '0';
						count = 100; //무한반복 탈출
					}

					else if (str[i][0] == '2' && str[i][1] == '0') //20이 입력된 경우, 정상케이스
					{
						if (i == 0) *row = 20;
						else *col = 20;
						count = 100; //무한반복 탈출
					}

					else
					{
						count = 1; //20이상의 정수가 입력된 경우
						break; //다른 문자열은 확인할 필요없음
					}
				}

				else
				{
					count = 1;//3자리이상의 수
					break; //다른 문자열은 확인할 필요 없음
				}
			}
		}

		if (count == 1) printf("4 이상 20 이하의 정수를 입력해주세요\n");
		if (count == 2) printf("한 자리의 수는 한 개의 문자로 표현해주세요\n");
		if (count == 100) break; //정상입력인경우
	}
}

void print_board(int row, int col, int** board, int score[], int count_turn, int** cursor1, int** cursor2) //보드판 출력
{
	int i, j;

	printf("[Player 1] %d : %d [Player 2]\n\n", score[1], score[2]); //점수 상황

	if (count_turn == 1) printf("[Player 1 ' s Turn]\n"); //플레이어 1의 턴
	else printf("[Player 2 ' s Turn]\n"); //플레이어 2의 턴
	
	printf("+");
	for(j = 0; j < col; j++)
		printf("---+");
	printf("\n");
	for (i = 0; i < row; i++) //행
	{
		printf("|");
		for (j = 0; j < col; j++) //열
		{
			if (count_turn == 1) //플레이어 1의 턴
			{
				if (board[i][j] == 0 && cursor1[i][j] == 0) //주인 없는 구역, 커서 x
					printf("   ");

				else if (board[i][j] == 0 && cursor1[i][j] == 1) //주인 없는 구역, 커서 o
					printf("[ ]");
				
				else if (board[i][j] == 1 && cursor1[i][j] == 0) //1의 구역, 커서 x
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
					printf(" @ ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}

				else if (board[i][j] == 1 && cursor1[i][j] == 1) //1의 구역, 커서 o
				{
					printf("[");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
					printf("@");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("]");
				}

				else if (board[i][j] == 2 && cursor1[i][j] == 0) //2의 구역, 커서 x
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					printf(" @ ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}

				else if (board[i][j] == 2 && cursor1[i][j] == 1) //2의 구역, 커서 o
				{
					printf("[");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					printf("@");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("]");
				}

				else if (board[i][j] == 3 && cursor1[i][j] == 0) //선택된 말, 커서 x
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
					printf(" @ ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}

				else if (board[i][j] == 3 && cursor1[i][j] == 1) //선택된 말, 커서 o
				{
					printf("[");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
					printf("@");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("]");
				}
			}

			else //플레이어 2의 턴
			{
				if (board[i][j] == 0 && cursor2[i][j] == 0) //주인 없는 구역, 커서 x
					printf("   ");

				else if (board[i][j] == 0 && cursor2[i][j] == 1) //주인 없는 구역, 커서 o
					printf("[ ]");

				else if (board[i][j] == 1 && cursor2[i][j] == 0) //1의 구역, 커서 x
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
					printf(" @ ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}

				else if (board[i][j] == 1 && cursor2[i][j] == 1) //1의 구역, 커서 o
				{
					printf("[");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
					printf("@");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("]");
				}

				else if (board[i][j] == 2 && cursor2[i][j] == 0) //2의 구역, 커서 x
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					printf(" @ ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}

				else if (board[i][j] == 2 && cursor2[i][j] == 1) //2의 구역, 커서 o
				{
					printf("[");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					printf("@");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("]");
				}

				else if (board[i][j] == 3 && cursor2[i][j] == 0) //선택된 말, 커서 x
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
					printf(" @ ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}

				else if (board[i][j] == 3 && cursor2[i][j] == 1) //선택된 말, 커서 o
				{
					printf("[");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
					printf("@");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("]");
				}
			}
			printf("|");
		}
		printf("\n+");
		for(j = 0; j < col; j++)
			printf("---+");
		printf("\n");
	}
}

void turn(int** board, int** cursor1, int** cursor2, int* count_turn, int* x1, int* y1, int* x2, int* y2, int row, int col, int* count_choice, int score[]) //플레이어의 턴을 진행해주는 함수
{
	char input;
	int x0 = 0, y0 = 0, i, j;
	float d;
	int corx[3], cory[3];

	input = _getch();

	if (*count_turn == 1) //플레이어 1의 턴
	{
		if (input == 'i') //위로이동
		{
			if (*y1 != 0) //0이면 막혀서 위로 이동 못함
			{
				cursor1[*y1][*x1] = 0; //커서끄기
				*y1 = *y1 - 1; //y1 좌표 변경
				cursor1[*y1][*x1] = 1; //새 좌표에 커서켜기
			}
		}

		else if (input == 'k') //아래로이동
		{
			if (*y1 != (row - 1)) //(row-1)이면 막혀서 아래로 이동 못함
			{
				cursor1[*y1][*x1] = 0; //커서끄기
				*y1 = *y1 + 1; //y1 좌표 변경
				cursor1[*y1][*x1] = 1; //새 좌표에 커서켜기 
			}
		}

		else if (input == 'j') //왼쪽으로 이동
		{
			if (*x1 != 0) //0이면 막혀서 왼쪽으로 이동 못함
			{
				cursor1[*y1][*x1] = 0; //커서끄기
				*x1 = *x1 - 1; //x1 좌표 변경
				cursor1[*y1][*x1] = 1; //새 좌표에 커서켜기
			}
		}

		else if (input == 'l') //오른쪽으로 이동
		{
			if (*x1 != (col - 1)) //(col-1)이면 막혀서 오른쪽으로 이동 못함
			{
				cursor1[*y1][*x1] = 0; //커서끄기
				*x1 = *x1 + 1; //x1 좌표 변경
				cursor1[*y1][*x1] = 1; //새 좌표에 커서켜기
			}
		}

		else if (input == ' ') //스페이스바가 입력
		{
			if (*count_choice == 0) //처음 선택되는거라면
				space(board, *x1, *y1, *count_turn, count_choice); //이동할 말로 선택

			else //이미 이동할 말이 선택된 상태라면
			{
				for (i = 0; i < row; i++)
				{
					for (j = 0; j < col; j++)
					{
						if (board[i][j] == 3) //원래 선택된 곳
							x0 = j, y0 = i; //좌표 저장
					}
				}

				if (test(board, *x1, *y1, *count_turn, x0, y0, &d) == 0) //문제가 없다면
				{
					if (d < 1.5) //sqrt2까지 1칸, 복제
					{
						board[*y1][*x1] = *count_turn; //현재위치를 플레이어의 영역으로 변경
						board[y0][x0] = *count_turn; //선택되어있던 영역 색깔 원래대로
						score[1] += 1; //내점수 업
					}

					else //2칸, 이동
					{
						board[*y1][*x1] = *count_turn; //현재위치를 플레이어의 영역으로 변경
						board[y0][x0] = 0; //선택되어있던 영역은 중립영역으로 되돌아감
					}

					for (i = 0; i < 3; i++) //현재좌표 기준 1칸범위 x값 저장
						corx[i] = *x1 - 1 + i;

					for (i = 0; i < 3; i++) //현재좌표 기준 1칸범위 y값 저장
						cory[i] = *y1 - 1 + i;

					for (i = 0; i < 3; i++)
					{
						for (j = 0; j < 3; j++)
						{
							if (corx[j] >= 0 && corx[j] <= col - 1 && cory[i] >= 0 && cory[i] <= row - 1) //좌표가 보드판 위에 있고
							{
								if (board[cory[i]][corx[j]] == 2) //상대 영역이면
								{
									board[cory[i]][corx[j]] = *count_turn; //내영역으로 바꿔줌
									score[1] += 1; //내점수 올리고
									score[2] -= 1; //상대점수 내리고
								}
							}
						}
					}

					*count_choice = 0; //카운터 초기화
					*count_turn = *count_turn % 2 + 1; //플레이어 순서변경
				}
			}
		}
	}

	else //플레이어 2의 턴
	{
		if (input == 'i') //위로이동
		{
			if (*y2 != 0) //0이면 막혀서 위로 이동 못함
			{
				cursor2[*y2][*x2] = 0; //커서끄기
				*y2 = *y2 - 1; //y2 좌표 변경
				cursor2[*y2][*x2] = 1; //새 좌표에 커서켜기
			}
		}

		else if (input == 'k') //아래로이동
		{
			if (*y2 != (row - 1)) //(row-1)이면 막혀서 아래로 이동 못함
			{
				cursor2[*y2][*x2] = 0; //커서끄기
				*y2 = *y2 + 1; //y2 좌표 변경
				cursor2[*y2][*x2] = 1; //새 좌표에 커서켜기 
			}
		}

		else if (input == 'j') //왼쪽으로 이동
		{
			if (*x2 != 0) //0이면 막혀서 왼쪽으로 이동 못함
			{
				cursor2[*y2][*x2] = 0; //커서끄기
				*x2 = *x2 - 1; //x2 좌표 변경
				cursor2[*y2][*x2] = 1; //새 좌표에 커서켜기
			}
		}

		else if (input == 'l') //오른쪽으로 이동
		{
			if (*x2 != (col - 1)) //(col-1)이면 막혀서 오른쪽으로 이동 못함
			{
				cursor2[*y2][*x2] = 0; //커서끄기
				*x2 = *x2 + 1; //x2 좌표 변경
				cursor2[*y2][*x2] = 1; //새 좌표에 커서켜기
			}
		}

		else if (input == ' ') //스페이스바가 입력
		{
			if (*count_choice == 0) //처음 선택되는거라면
				space(board, *x2, *y2, *count_turn, count_choice); //이동할 말로 선택

			else //이미 이동할 말이 선택된 상태라면
			{
				for (i = 0; i < row; i++)
				{
					for (j = 0; j < col; j++)
					{
						if (board[i][j] == 3) //원래 선택된 곳
							x0 = j, y0 = i; //좌표 저장
					}
				}

				if (test(board, *x2, *y2, *count_turn, x0, y0, &d) == 0) //문제가 없다면
				{
					if (d < 1.5) //sqrt2까지 1칸, 복제
					{
						board[*y2][*x2] = *count_turn; //현재위치를 플레이어의 영역으로 변경
						board[y0][x0] = *count_turn; //선택되어있던 영역 색깔 원래대로
						score[2] += 1; //내점수 업
					}

					else //2칸, 이동
					{
						board[*y2][*x2] = *count_turn; //현재위치를 플레이어의 영역으로 변경
						board[y0][x0] = 0; //선택되어있던 영역은 중립영역으로 되돌아감
					}

					for (i = 0; i < 3; i++) //현재좌표 기준 1칸범위 x값 저장
						corx[i] = *x2 - 1 + i;

					for (i = 0; i < 3; i++) //현재좌표 기준 1칸범위 y값 저장
						cory[i] = *y2 - 1 + i;

					for (i = 0; i < 3; i++)
					{
						for (j = 0; j < 3; j++)
						{
							if (corx[j] >= 0 && corx[j] <= col - 1 && cory[i] >= 0 && cory[i] <= row - 1) //좌표가 보드판 위에 있고
							{
								if (board[cory[i]][corx[j]] == 1) //상대 영역이면
								{
									board[cory[i]][corx[j]] = *count_turn; //내영역으로 바꿔줌
									score[2] += 1; //내점수 올리고
									score[1] -= 1; //상대점수 내리고
								}
							}
						}
					}

					*count_choice = 0; //카운터 초기화
					*count_turn = *count_turn % 2 + 1; //플레이어 순서변경
				}
			}
		}
	}
}

void gotoxy(int x, int y) //커서 이동 함수
{
	COORD pos = { x + 2 , y + 4 }; //(0,0)이 보드판의 좌상귀가 되도록 세팅
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); //커서이동
}

void space(int** board, int x, int y, int count_turn, int* count_choice) //말 선택 함수
{
	if (board[y][x] == count_turn) // 그 플레이어의 말이 맞으면
	{
		board[y][x] = 3; //말 선택
		*count_choice = 1; //카운터 켜기
	}
}

int test(int** board, int x, int y, int count_turn, int x0, int y0, float* d) //적절한 선택인지 테스트하는 함수
{
	if (board[y][x] == count_turn || board[y][x] == 3) //플레이어 자신의 구역을 선택한 경우
	{
		board[y0][x0] = count_turn; //취소
		board[y][x] = 3; //여기로 재선택
		return 1; //문제가 있었으므로 1반환
	}

	else if (board[y][x] == 0) //빈구역이면
	{
		if (test_distance(x0, y0, x, y, d) == 0) //이동 가능하면
			return 0; //문제가 없으므로 0반환
		
		else //이동 불가능하면
			return 1; //문제가 있으므로 1반환
	}

	else //다른플레이어의 구역이면
		return 1; //문제있으므로 1반환
}

int test_distance(int x0, int y0, int x, int y, float* d) //거리 체크 함수
{
	int dx = abs(x0 - x); //x좌표 사이 거리 저장
	int dy = abs(y0 - y); //y좌표 사이 거리 저장

	*d = (float)sqrt(pow(dx, 2) + pow(dy, 2)); //거리 저장

	if (dx == 0 && dy != 0)
	{
		if (dy <= 2) //거리가 2이하이면
			return 0; //이동가능

		else //크면
			return 1; //이동불가능
	}

	else if (dx != 0 && dy == 0)
	{
		if (dx <= 2) //거리가 2이하이면
			return 0; //이동가능

		else //크면
			return 1; //이동불가능
	}

	else if (dx != 0 && dy != 0)
	{
		if (*d < 3) //최대거리가 sqrt8이므로 3보다 작으면 통과가능
			return 0; //이동가능

		else //크면
			return 1; //이동불가능
	}

	return 100; //에러용
}

int test_move(int** board, int* count_turn, int row, int col, int score[]) //플레이어의 말이 움직일 곳이 있는지 테스트하는 함수
{
	int i, j, x, y, count = 0, num = 0;
	int testx[5], testy[5];

	if (*count_turn == 1) //플레이어 1의 턴인 경우
	{
		for (i = 0; i < row; i++)
		{
			for (j = 0; j < col; j++)
			{
				if (board[i][j] == 1) //플레이어1의 영역을 찾으면
				{
					for (x = 0; x < 5; x++) //현재좌표 기준 2칸 범위 x값 저장
						testx[x] = j - 2 + x;

					for (y = 0; y < 5; y++) //현재 좌표 기준 2칸 범위 y값 저장
						testy[y] = i - 2 + y;

					for (y = 0; y < 5; y++)
					{
						for (x = 0; x < 5; x++)
						{
							if (testx[x] >= 0 && testx[x] <= col - 1 && testy[y] >= 0 && testy[y] <= row - 1) //좌표가 보드판 위에 있는 경우
							{
								if (board[testy[y]][testx[x]] != 0) //빈 영역이 아니면 
									count += 1;
							}

							else //좌표가 보드판 위에 없는 경우
								count += 1;
						}
					}

					if(count == 25) //주변 2칸범위 모든영역으로 이동할 수 없는
						num += 1; //말의 개수를 카운트
				}

				count = 0; //재활용위해 초기화
			}
		}
		//여기까지 하면 num에 이동불가능한 말의 개수 저장됨

		if (num == score[1]) //모든 말이 이동불가능하다면 (score는 모든 말의 개수와 같음)
		{
			*count_turn = *count_turn % 2 + 1; //플레이어 순서 변경
			return 1; //특이케이스
		}
	}

	else if (*count_turn == 2) //플레이어 2의 턴인 경우
	{
		for (i = 0; i < row; i++)
		{
			for (j = 0; j < col; j++)
			{
				if (board[i][j] == 2) //플레이어2의 영역을 찾으면
				{
					for (x = 0; x < 5; x++) //현재좌표 기준 2칸 범위 x값 저장
						testx[x] = j - 2 + x;

					for (y = 0; y < 5; y++) //현재 좌표 기준 2칸 범위 y값 저장
						testy[y] = i - 2 + y;

					for (y = 0; y < 5; y++)
					{
						for (x = 0; x < 5; x++)
						{
							if (testx[x] >= 0 && testx[x] <= col - 1 && testy[y] >= 0 && testy[y] <= row - 1) //좌표가 보드판 위에 있는 경우
							{
								if (board[testy[y]][testx[x]] != 0) //빈 영역이 아니면 
									count += 1;
							}

							else //좌표가 보드판 위에 없는 경우
								count += 1;
						}
					}

					if (count == 25) //주변 2칸범위 모든영역으로 이동할 수 없는
						num += 1; //말의 개수를 카운트
				}

				count = 0; //재활용위해 초기화
			}
		}
		//여기까지 하면 num에 이동불가능한 말의 개수 저장됨

		if (num == score[2]) //모든 말이 이동불가능하다면 (score는 모든 말의 개수와 같음)
		{
			*count_turn = *count_turn % 2 + 1; //플레이어 순서 변경
			return 1; //특이케이스
		}
	}

	return 0; //정상케이스
}