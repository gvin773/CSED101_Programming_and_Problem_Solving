#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>

void print_score(int score1, int socre2); //���ھ� ���
void print_board(int number[][4], int temp_open[][4], int opened[][4]); //������ ���
int check_input(int temp_open[][4], int opened[][4], int* x1, int* y1, int* x2, int* y2); //������ ��ĭ �����ִ��� Ȯ��
void init_board(int number[][4]); //������ �ʱ�ȭ
int player_turn(int* score1, int* score2, int* turn, int number[][4], int temp_open[][4], int opened[][4]); //player�� ���� �����ϴ� �Լ�
int search(int array[], int last, int target); //�迭�� ���ϴ� ���� �ִ��� Ž���ϴ� �Լ�
int cmp(int number[][4], int x1, int y1, int x2, int y2); //���� �Լ�
int end_check(int opened[][4]); //������ ����Ǿ����� Ȯ���ϴ� �Լ�
void reset(int counter[][4]); //ī���� �ʱ�ȭ

int main()
{
	char re;
	int i, j, regame = 1;
	int turn = 0;
	int score1, score2;
	int correction;
	int number[4][4];
	int temp_open[4][4], opened[4][4] = { 0 }; //���¿��� ���� �ӽ�ī����, ����ī����
	srand((unsigned int)time(NULL)); //�õ尪 ������ �ο�

	//���ӽ���
	while (regame) //���ӽ���
	{
		score1 = 0, score2 = 0;
		turn = rand() % 2 + 1; //������ �������� ������
		for (i = 0; i < 4; i++) //������ ���۵Ǹ� ó������ ���ڸ� ��� ���������
		{
			for (j = 0; j < 4; j++)
				temp_open[i][j] = 1;
		}

		reset(opened); //������ ����� �ɰ�� ����ī���͵� �ʱ�ȭ���Ѿ� �ϹǷ�
		printf("Memorize the board number!!\n\n");
		init_board(number);
		print_board(number, temp_open, opened);
		Sleep(3000);
		system("cls");
		print_board(number, temp_open, opened); //�������� ������
		//������� �����غ����~ù��° �÷��̾��� �� ������
		while (1)
		{
			correction = player_turn(&score1, &score2, &turn, number, temp_open, opened); //�÷��̾��� ���� ����
			if (correction == 1) //�ȿ��� ���δٸ� 2�� + ���Ƽ� ����ȹ��
			{
				printf("\n");
				print_board(number, temp_open, opened);
				printf("player %d get score!!\n", turn);
				print_score(score1, score2);
				if (end_check(opened)) //������ �������
				{
					if (score1 == score2) printf("Game End! Draw!\n");
					else if (score1 > score2) printf("Game End! Player 1 wins!\n");
					else printf("Game End! Player 2 wins!\n");
					//���Ӱ�� ���
					printf("Continue? (y/n) "); //��������� ���� üũ
					scanf(" %c", &re); //" %c"�� ����Ͽ� ���ۿ� �����ִ� ���๮�� ����
					if (re == 'y')
					{
						printf("\nNew Game!\n");
						break; //���������
					}
					else if (re == 'n')
					{
						printf("Good bye\n\n");
						regame = 0;
						break; //��� while���� Ż���Ͽ� ���α׷��� ������ ����
					}
				}
				else //������ ������ ���� ���
					printf("player %d get one more chance!!\n", turn);
				turn += 1; //�����÷��̾ �ѹ����ؾ��ϹǷ�
			}

			else if (correction == 2) //���δٸ� 2�� + �޶� ����ȹ�����
			{
				printf("\n");
				print_board(number, temp_open, opened); //ī�� 2�� 3�ʵ��� ������
				printf("Mismatch!\n");
				Sleep(3000);
				system("cls");
				print_board(number, temp_open, opened); //�������� ������
			}
			else if (correction == 0) //�÷��̾ �Է� �߸�������
			{
				reset(temp_open); //��������¿� �ӽ�ī���� ������ ����Ǿ� �ִµ� �� ���� �������� ������� �����Ƿ� ���⼭ �ʱ�ȭ�������
				turn += 1; //�ٽ��ؾ��ϹǷ�
			}
		}
	}
	return 0;
}

void print_score(int score1, int score2) //���ھ� ���
{
	printf("---------------------------\n");
	printf("[player 1] %d : %d [player2]\n", score1, score2);
	printf("---------------------------\n\n");
}

void print_board(int number[][4], int temp_open[][4], int opened[][4]) //������ ���
{
	int i, j;

	printf("     0   1   2   3\n");
	printf("   +---+---+---+---+\n");
	for (i = 0; i < 4; i++)
	{
		printf(" %d |", i);
		for (j = 0; j < 4; j++)
		{
			if (temp_open[i][j] == 0 && opened[i][j] == 0) //ī���Ͱ� ��� 0�̸�
				printf(" * |");

			else //ī���� �� �ϳ� �̻��� 1�̸�
				printf(" %d |", number[i][j]);
		}
		printf("\n"); //�ٹٲ�
		printf("   +---+---+---+---+\n");
	}

	reset(temp_open); //�ӽ�ī���� �ʱ�ȭ
	printf("\n");
}

int check_input(int temp_open[][4], int opened[][4], int* x1, int* y1, int* x2, int* y2) //������ ��ĭ �����ִ��� Ȯ��
{
	if (*x1 == *x2 && *y1 == *y2) //������ǥ 2�� �Է�
	{
		printf("\nsame card... try again..\n");
		return 1;
	}
	else //�ٸ���ǥ 2�� �Է�
	{
		if (temp_open[*x1][*y1] != opened[*x1][*y1] && temp_open[*x2][*y2] != opened[*x2][*y2]) //�����Է¹��� ��ǥ�� ī�尡 ��� ����������
		{
			temp_open[*x1][*y1] = 1;
			temp_open[*x1][*y1] = 1;
			return 0;
		}
		else //�Է¹��� ��ǥ�� ī�� ���� �ϳ��̻��� �̹� ���� �ִ� ���
		{
			printf("\nalready open... try again..\n");
			return 1;
		}
	}
}

void init_board(int number[][4]) //������ �ʱ�ȭ
{
	int i, j;
	int temp[16] = { 0 }; //���������ӽ������ ����
	int t;

	for (i = 0; i < 16; i++) //1-8���� ���� 2���� ����ִ� �����迭 ����
	{
		while (1)
		{
			t = rand() % 8 + 1;
			if (search(temp, i, t) == 0) //���ų� 1����
			{
				temp[i] = t;
				break;
			}
		}
	}

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
			number[i][j] = temp[4 * i + j]; //1���� �迭�� 2���� �迭�� ����
	}
}

int player_turn(int* score1, int* score2, int* turn, int number[][4], int temp_open[][4], int opened[][4]) //player�� ���� �����ϴ� �Լ�
{
	int x1, y1, x2, y2;

	*turn = *turn % 2 + 1; //1�� 2���ǰ� 2�� 1�̵�

	printf("[player %d] : ", *turn);
	scanf("%d%d%d%d", &x1, &y1, &x2, &y2); //��ǥ �Է� ����

	temp_open[x1][y1] = 1;
	temp_open[x2][y2] = 1;

	if (check_input(temp_open, opened, &x1, &y1, &x2, &y2) == 0) //�ȿ��� ���δٸ� 2�� �������
	{
		if (cmp(number, x1, y1, x2, y2)) //�� ���� ������
		{
			opened[x1][y1] = 1;
			opened[x2][y2] = 1; //�� ī�� ���� ����

			if (*turn == 1) *score1 += 1;
			else *score2 += 1; //����ȹ��

			return 1;
		}

		else //�� ���� �ٸ���
			return 2;
	}
	else //�Է��� �߸��Ǹ�
		return 0;
}

int search(int array[], int last, int target) //�迭�� ���ϴ� ���� �ִ��� Ž���ϴ� �Լ�
{ //2����1, ���ų� 1���� 0 ��ȯ
	int i;
	int count = 0;

	for (i = 0; i < last; i++) //�迭������ Ž��
	{
		if (array[i] == target) //������ ī��Ʈ�� +1
			count += 1;
	}

	if (count == 2) //�������ڴ� 2�������� �����ϹǷ�
		return 1;
	else
		return 0;
}

int cmp(int number[][4], int x1, int y1, int x2, int y2) //���� �Լ�
{
	if (number[x1][y1] == number[x2][y2]) return 1; //������

	else return 0; //�ٸ���
}

int end_check(int opened[][4]) //������ ����Ǿ����� Ȯ���ϴ� �Լ�
{
	int i, j, end_count = 1;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (opened[i][j] == 0) //���� �ȵ����� ī�� �� ������
			{
				end_count = 0;
				i = 10;
				break; //�� Ȯ���� �ʿ�����Ƿ� ��� �ݺ����� Ż��
			}
		}
	}

	if (end_count) return 1; //��� ī�尡 ���������
	else return 0; //���� �ȵ����� ī�尡 �ִ� ���
}

void reset(int counter[][4]) //ī���� �ʱ�ȭ
{
	int i, j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
			counter[i][j] = 0;
	}
}