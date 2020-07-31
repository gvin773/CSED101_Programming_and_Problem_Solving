#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>

void print_score(int score1, int socre2); //스코어 출력
void print_board(int number[][4], int temp_open[][4], int opened[][4]); //보드판 출력
int check_input(int temp_open[][4], int opened[][4], int* x1, int* y1, int* x2, int* y2); //보드판 두칸 열수있는지 확인
void init_board(int number[][4]); //보드판 초기화
int player_turn(int* score1, int* score2, int* turn, int number[][4], int temp_open[][4], int opened[][4]); //player의 턴을 진행하는 함수
int search(int array[], int last, int target); //배열에 원하는 값이 있는지 탐색하는 함수
int cmp(int number[][4], int x1, int y1, int x2, int y2); //값비교 함수
int end_check(int opened[][4]); //게임이 종료되었는지 확인하는 함수
void reset(int counter[][4]); //카운터 초기화

int main()
{
	char re;
	int i, j, regame = 1;
	int turn = 0;
	int score1, score2;
	int correction;
	int number[4][4];
	int temp_open[4][4], opened[4][4] = { 0 }; //오픈여부 관련 임시카운터, 영구카운터
	srand((unsigned int)time(NULL)); //시드값 가변성 부여

	//게임시작
	while (regame) //게임시작
	{
		score1 = 0, score2 = 0;
		turn = rand() % 2 + 1; //누구의 선공인지 정해줌
		for (i = 0; i < 4; i++) //게임이 시작되면 처음에는 숫자를 모두 보여줘야함
		{
			for (j = 0; j < 4; j++)
				temp_open[i][j] = 1;
		}

		reset(opened); //게임이 재시작 될경우 영구카운터도 초기화시켜야 하므로
		printf("Memorize the board number!!\n\n");
		init_board(number);
		print_board(number, temp_open, opened);
		Sleep(3000);
		system("cls");
		print_board(number, temp_open, opened); //가린보드 보여줌
		//여기까지 게임준비과정~첫번째 플레이어의 턴 시작전
		while (1)
		{
			correction = player_turn(&score1, &score2, &turn, number, temp_open, opened); //플레이어의 턴이 시작
			if (correction == 1) //안열린 서로다른 2개 + 같아서 점수획득
			{
				printf("\n");
				print_board(number, temp_open, opened);
				printf("player %d get score!!\n", turn);
				print_score(score1, score2);
				if (end_check(opened)) //게임이 끝난경우
				{
					if (score1 == score2) printf("Game End! Draw!\n");
					else if (score1 > score2) printf("Game End! Player 1 wins!\n");
					else printf("Game End! Player 2 wins!\n");
					//게임결과 출력
					printf("Continue? (y/n) "); //게임재시작 여부 체크
					scanf(" %c", &re); //" %c"를 사용하여 버퍼에 남아있던 개행문자 무시
					if (re == 'y')
					{
						printf("\nNew Game!\n");
						break; //게임재시작
					}
					else if (re == 'n')
					{
						printf("Good bye\n\n");
						regame = 0;
						break; //모든 while문을 탈출하여 프로그램을 완전히 종료
					}
				}
				else //게임이 끝나지 않은 경우
					printf("player %d get one more chance!!\n", turn);
				turn += 1; //지금플레이어가 한번더해야하므로
			}

			else if (correction == 2) //서로다른 2개 + 달라서 점수획득실패
			{
				printf("\n");
				print_board(number, temp_open, opened); //카드 2장 3초동안 보여줌
				printf("Mismatch!\n");
				Sleep(3000);
				system("cls");
				print_board(number, temp_open, opened); //가린보드 보여줌
			}
			else if (correction == 0) //플레이어가 입력 잘못했으면
			{
				reset(temp_open); //보드판출력에 임시카운터 리셋이 내장되어 있는데 이 경우는 보드판을 출력하지 않으므로 여기서 초기화해줘야함
				turn += 1; //다시해야하므로
			}
		}
	}
	return 0;
}

void print_score(int score1, int score2) //스코어 출력
{
	printf("---------------------------\n");
	printf("[player 1] %d : %d [player2]\n", score1, score2);
	printf("---------------------------\n\n");
}

void print_board(int number[][4], int temp_open[][4], int opened[][4]) //보드판 출력
{
	int i, j;

	printf("     0   1   2   3\n");
	printf("   +---+---+---+---+\n");
	for (i = 0; i < 4; i++)
	{
		printf(" %d |", i);
		for (j = 0; j < 4; j++)
		{
			if (temp_open[i][j] == 0 && opened[i][j] == 0) //카운터가 모두 0이면
				printf(" * |");

			else //카운터 중 하나 이상이 1이면
				printf(" %d |", number[i][j]);
		}
		printf("\n"); //줄바꿈
		printf("   +---+---+---+---+\n");
	}

	reset(temp_open); //임시카운터 초기화
	printf("\n");
}

int check_input(int temp_open[][4], int opened[][4], int* x1, int* y1, int* x2, int* y2) //보드판 두칸 열수있는지 확인
{
	if (*x1 == *x2 && *y1 == *y2) //같은좌표 2개 입력
	{
		printf("\nsame card... try again..\n");
		return 1;
	}
	else //다른좌표 2개 입력
	{
		if (temp_open[*x1][*y1] != opened[*x1][*y1] && temp_open[*x2][*y2] != opened[*x2][*y2]) //새로입력받은 좌표의 카드가 모두 닫혀있으면
		{
			temp_open[*x1][*y1] = 1;
			temp_open[*x1][*y1] = 1;
			return 0;
		}
		else //입력받은 좌표의 카드 둘중 하나이상이 이미 열려 있던 경우
		{
			printf("\nalready open... try again..\n");
			return 1;
		}
	}
}

void init_board(int number[][4]) //보드판 초기화
{
	int i, j;
	int temp[16] = { 0 }; //랜덤숫자임시저장용 변수
	int t;

	for (i = 0; i < 16; i++) //1-8까지 숫자 2개씩 들어있는 랜덤배열 생성
	{
		while (1)
		{
			t = rand() % 8 + 1;
			if (search(temp, i, t) == 0) //없거나 1개면
			{
				temp[i] = t;
				break;
			}
		}
	}

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
			number[i][j] = temp[4 * i + j]; //1차원 배열을 2차원 배열에 저장
	}
}

int player_turn(int* score1, int* score2, int* turn, int number[][4], int temp_open[][4], int opened[][4]) //player의 턴을 진행하는 함수
{
	int x1, y1, x2, y2;

	*turn = *turn % 2 + 1; //1은 2가되고 2는 1이됨

	printf("[player %d] : ", *turn);
	scanf("%d%d%d%d", &x1, &y1, &x2, &y2); //좌표 입력 받음

	temp_open[x1][y1] = 1;
	temp_open[x2][y2] = 1;

	if (check_input(temp_open, opened, &x1, &y1, &x2, &y2) == 0) //안열린 서로다른 2개 골랐으면
	{
		if (cmp(number, x1, y1, x2, y2)) //두 수가 같으면
		{
			opened[x1][y1] = 1;
			opened[x2][y2] = 1; //두 카드 영구 오픈

			if (*turn == 1) *score1 += 1;
			else *score2 += 1; //점수획득

			return 1;
		}

		else //두 수가 다르면
			return 2;
	}
	else //입력이 잘못되면
		return 0;
}

int search(int array[], int last, int target) //배열에 원하는 값이 있는지 탐색하는 함수
{ //2개면1, 없거나 1개면 0 반환
	int i;
	int count = 0;

	for (i = 0; i < last; i++) //배열끝까지 탐색
	{
		if (array[i] == target) //있으면 카운트값 +1
			count += 1;
	}

	if (count == 2) //같은숫자는 2개까지만 가능하므로
		return 1;
	else
		return 0;
}

int cmp(int number[][4], int x1, int y1, int x2, int y2) //값비교 함수
{
	if (number[x1][y1] == number[x2][y2]) return 1; //같으면

	else return 0; //다르면
}

int end_check(int opened[][4]) //게임이 종료되었는지 확인하는 함수
{
	int i, j, end_count = 1;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (opened[i][j] == 0) //아직 안뒤집힌 카드 가 있으면
			{
				end_count = 0;
				i = 10;
				break; //더 확인할 필요없으므로 모든 반복문을 탈출
			}
		}
	}

	if (end_count) return 1; //모든 카드가 뒤집힌경우
	else return 0; //아직 안뒤집힌 카드가 있는 경우
}

void reset(int counter[][4]) //카운터 초기화
{
	int i, j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
			counter[i][j] = 0;
	}
}