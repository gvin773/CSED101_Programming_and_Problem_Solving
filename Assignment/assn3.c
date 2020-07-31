#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <math.h>

void input(int* row, int* col); //����� ũ�⸦ �޴� �Լ�
void print_board(int row, int col, int** board, int score[], int count_turn, int** cursor1, int** cursor2); //������ ��� �Լ�
void turn(int** board, int** cursor1, int** cursor2, int* count_turn, int* x1, int* y1, int* x2, int* y2, int row, int col, int* count_choice, int score[]); //�÷��̾��� ���� �������ִ� �Լ�
void gotoxy(int x, int y); //Ŀ�� �̵� �Լ�
void space(int** board, int x, int y, int count_turn, int* count_choice); //�� ���� �Լ�
int test(int** board, int x, int y, int count_turn, int x0, int y0, float* d); //������ �������� �׽�Ʈ�ϴ� �Լ�
int test_distance(int x0, int y0, int x, int y, float* d); //�Ÿ� üũ �Լ�
int test_move(int** board, int* count_turn, int row, int col, int score[]); //�÷��̾��� ���� ������ ���� �ִ��� �׽�Ʈ�ϴ� �Լ�

int main()
{
	int row, col, i, j, count_turn, count_choice, break_count = 0, len;
	int x1, y1, x2, y2;
	int** board, **cursor1, **cursor2;
	int score[3] = { 0, 2, 2 }; //1�� �÷��̾�1, 2�� �÷��̾� 2
	char re[10000];

	while (1)
	{
		count_turn = 1, count_choice = 0, x1 = 0, y1 = 0, score[1] = 2, score[2] = 2; //�����ʱⰪ ����
		system("cls");
		printf("[ATAXX]\n");
		input(&row, &col); //��� ũ�� �Է¹���

		system("cls");

		board = (int**)calloc(row, sizeof(int*));
		for (i = 0; i < row; i++)
			*(board + i) = (int*)calloc(col, sizeof(int));
		//������ �����Ҵ�, 0�� ���ξ��� ����, 1�� 1�� ����, 2�� 2�� ����, 3�� ���õ� ��

		cursor1 = (int**)calloc(row, sizeof(int*));
		for (i = 0; i < row; i++)
			*(cursor1 + i) = (int*)calloc(col, sizeof(int));
		//Ŀ��1 �����Ҵ�, Ŀ��1�� �ִ� ���� 1 ���� ���� 0

		cursor2 = (int**)calloc(row, sizeof(int*));
		for (i = 0; i < row; i++)
			*(cursor2 + i) = (int*)calloc(col, sizeof(int));
		//Ŀ��2 �����Ҵ�, Ŀ��2�� �ִ� ���� 1 ���� ���� 0

		//�����غ�ܰ�
		for (i = 0; i < row; i++) //����, Ŀ��1, Ŀ��2 �ʱ�ȭ
			for (j = 0; j < col; j++)
			{
				board[i][j] = 0;
				cursor1[i][j] = 0;
				cursor2[i][j] = 0;
			}

		board[0][0] = 1, board[0][col - 1] = 1, cursor1[0][0] = 1; //�÷��̾�1 �ʱ� ����
		board[row - 1][0] = 2, board[row - 1][col - 1] = 2, cursor2[row - 1][col - 1] = 1; //�÷��̾�2 �ʱ� ����
		x2 = (col - 1), y2 = (row - 1);

		//���ӽ���
		print_board(row, col, board, score, count_turn, cursor1, cursor2);
		gotoxy(x1 * 4, y1 * 2);
		while (1)
		{
			turn(board, cursor1, cursor2, &count_turn, &x1, &y1, &x2, &y2, row, col, &count_choice, score);
			system("cls");
			print_board(row, col, board, score, count_turn, cursor1, cursor2);

			if (score[1] + score[2] == row * col || score[1] == 0 || score[2] == 0) //���� ������ ��� �Ǵ� ��� ���� �÷��̾��� ���� ���� ���
				break;

			if (test_move(board, &count_turn, row, col, score)) //���� ���ƿ� �÷��̾ ������ ���� ���ٸ� �ٸ� �÷��̾��� ������ �ٲ���
			{
				system("cls");
				print_board(row, col, board, score, count_turn, cursor1, cursor2);
			}

			if (count_turn == 1) //�÷��̾� 1�� �����̸�
				gotoxy(x1 * 4, y1 * 2);
			else //�÷��̾� 2�� �����̸�
				gotoxy(x2 * 4, y2 * 2);
		}

		gotoxy(-2, 2 * row);
		printf("\n");
		if (score[1] == score[2]) //���º�
			printf("Draw\n");
		else if (score[1] > score[2]) //�÷��̾� 1�� �¸�
			printf("Player 1 wins\n");

		else //�÷��̾� 2�� �¸�
			printf("Player 2 wins\n");

		for (i = 0; i < row; i++)
			free(board[i]);
		free(board);

		for (i = 0; i < row; i++)
			free(cursor1[i]);
		free(cursor1);

		for (i = 0; i < row; i++)
			free(cursor2[i]);
		free(cursor2); //�������� �����Ҵ� ����

		while (1)
		{
			len = 0;
			printf("Continue? (y/n)");
			scanf("%s", &re);

			for (i = 0; re[i] != '\0'; i++) //���ڿ� ������
				len += 1; //���ڿ� ���� ����

			if (len > 1) //2���� �̻��� �Է��� ���
				printf("y �Ǵ� n�� �Է����ּ���\n");

			else //1���ڸ� �Է��� ���
			{
				if (re[0] != 'n' && re[0] != 'y') //y�Ǵ� n�� �Էµ��� ���� ���
					printf("y�Ǵ� n�� �Է����ּ���\n");

				if (re[0] == 'n') //���������x
				{
					break_count = 1;
					break;
				}

				else if (re[0] == 'y') //���� ����� o
					break;
			}
		}

		if (break_count) //n�� �ԷµȰ�� ���α׷� ����
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
				if (str[i][j] < '0' || str[i][j] > '9') //���ڰ� ������
				{
					count = 1; //����ī��Ʈ
					break; //�����ʿ����
				}
			}
		}

		if(count != 1) //���ڷθ� �̷���� ���ڿ� Ȯ��
		{
			for (i = 0; i < 2; i++)
			{
				if (str[i][1] == '\0') //1�ڸ���
				{
					if (str[i][0] < '4')
					{
						count = 1; //4�̸��� ���� �ԷµȰ��
						break; //�ٸ� ���ڿ��� Ȯ���� �ʿ����
					}

					else //4�̻��� ���ڸ���, �������̽�
					{
						if (i == 0) *row = str[i][0] - '0'; //row�� ������
						else *col = str[i][0] - '0'; //col�� ������
						count = 100; //���ѹݺ� Ż��
					}
				}

				else if (str[i][2] == '\0') //2�ڸ���
				{
					if (str[i][0] == '0')
					{
						count = 2; //01, 02 �̷����� �Է��ΰ��
						break; //�ٸ� ���ڿ��� Ȯ���� �ʿ����
					}

					else if (str[i][0] == '1') //1���� ����, �������̽�
					{
						if (i == 0) *row = 10 + str[i][1] - '0';
						else *col = 10 + str[i][1] - '0';
						count = 100; //���ѹݺ� Ż��
					}

					else if (str[i][0] == '2' && str[i][1] == '0') //20�� �Էµ� ���, �������̽�
					{
						if (i == 0) *row = 20;
						else *col = 20;
						count = 100; //���ѹݺ� Ż��
					}

					else
					{
						count = 1; //20�̻��� ������ �Էµ� ���
						break; //�ٸ� ���ڿ��� Ȯ���� �ʿ����
					}
				}

				else
				{
					count = 1;//3�ڸ��̻��� ��
					break; //�ٸ� ���ڿ��� Ȯ���� �ʿ� ����
				}
			}
		}

		if (count == 1) printf("4 �̻� 20 ������ ������ �Է����ּ���\n");
		if (count == 2) printf("�� �ڸ��� ���� �� ���� ���ڷ� ǥ�����ּ���\n");
		if (count == 100) break; //�����Է��ΰ��
	}
}

void print_board(int row, int col, int** board, int score[], int count_turn, int** cursor1, int** cursor2) //������ ���
{
	int i, j;

	printf("[Player 1] %d : %d [Player 2]\n\n", score[1], score[2]); //���� ��Ȳ

	if (count_turn == 1) printf("[Player 1 ' s Turn]\n"); //�÷��̾� 1�� ��
	else printf("[Player 2 ' s Turn]\n"); //�÷��̾� 2�� ��
	
	printf("+");
	for(j = 0; j < col; j++)
		printf("---+");
	printf("\n");
	for (i = 0; i < row; i++) //��
	{
		printf("|");
		for (j = 0; j < col; j++) //��
		{
			if (count_turn == 1) //�÷��̾� 1�� ��
			{
				if (board[i][j] == 0 && cursor1[i][j] == 0) //���� ���� ����, Ŀ�� x
					printf("   ");

				else if (board[i][j] == 0 && cursor1[i][j] == 1) //���� ���� ����, Ŀ�� o
					printf("[ ]");
				
				else if (board[i][j] == 1 && cursor1[i][j] == 0) //1�� ����, Ŀ�� x
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
					printf(" @ ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}

				else if (board[i][j] == 1 && cursor1[i][j] == 1) //1�� ����, Ŀ�� o
				{
					printf("[");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
					printf("@");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("]");
				}

				else if (board[i][j] == 2 && cursor1[i][j] == 0) //2�� ����, Ŀ�� x
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					printf(" @ ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}

				else if (board[i][j] == 2 && cursor1[i][j] == 1) //2�� ����, Ŀ�� o
				{
					printf("[");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					printf("@");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("]");
				}

				else if (board[i][j] == 3 && cursor1[i][j] == 0) //���õ� ��, Ŀ�� x
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
					printf(" @ ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}

				else if (board[i][j] == 3 && cursor1[i][j] == 1) //���õ� ��, Ŀ�� o
				{
					printf("[");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
					printf("@");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("]");
				}
			}

			else //�÷��̾� 2�� ��
			{
				if (board[i][j] == 0 && cursor2[i][j] == 0) //���� ���� ����, Ŀ�� x
					printf("   ");

				else if (board[i][j] == 0 && cursor2[i][j] == 1) //���� ���� ����, Ŀ�� o
					printf("[ ]");

				else if (board[i][j] == 1 && cursor2[i][j] == 0) //1�� ����, Ŀ�� x
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
					printf(" @ ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}

				else if (board[i][j] == 1 && cursor2[i][j] == 1) //1�� ����, Ŀ�� o
				{
					printf("[");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
					printf("@");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("]");
				}

				else if (board[i][j] == 2 && cursor2[i][j] == 0) //2�� ����, Ŀ�� x
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					printf(" @ ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}

				else if (board[i][j] == 2 && cursor2[i][j] == 1) //2�� ����, Ŀ�� o
				{
					printf("[");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					printf("@");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("]");
				}

				else if (board[i][j] == 3 && cursor2[i][j] == 0) //���õ� ��, Ŀ�� x
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
					printf(" @ ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}

				else if (board[i][j] == 3 && cursor2[i][j] == 1) //���õ� ��, Ŀ�� o
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

void turn(int** board, int** cursor1, int** cursor2, int* count_turn, int* x1, int* y1, int* x2, int* y2, int row, int col, int* count_choice, int score[]) //�÷��̾��� ���� �������ִ� �Լ�
{
	char input;
	int x0 = 0, y0 = 0, i, j;
	float d;
	int corx[3], cory[3];

	input = _getch();

	if (*count_turn == 1) //�÷��̾� 1�� ��
	{
		if (input == 'i') //�����̵�
		{
			if (*y1 != 0) //0�̸� ������ ���� �̵� ����
			{
				cursor1[*y1][*x1] = 0; //Ŀ������
				*y1 = *y1 - 1; //y1 ��ǥ ����
				cursor1[*y1][*x1] = 1; //�� ��ǥ�� Ŀ���ѱ�
			}
		}

		else if (input == 'k') //�Ʒ����̵�
		{
			if (*y1 != (row - 1)) //(row-1)�̸� ������ �Ʒ��� �̵� ����
			{
				cursor1[*y1][*x1] = 0; //Ŀ������
				*y1 = *y1 + 1; //y1 ��ǥ ����
				cursor1[*y1][*x1] = 1; //�� ��ǥ�� Ŀ���ѱ� 
			}
		}

		else if (input == 'j') //�������� �̵�
		{
			if (*x1 != 0) //0�̸� ������ �������� �̵� ����
			{
				cursor1[*y1][*x1] = 0; //Ŀ������
				*x1 = *x1 - 1; //x1 ��ǥ ����
				cursor1[*y1][*x1] = 1; //�� ��ǥ�� Ŀ���ѱ�
			}
		}

		else if (input == 'l') //���������� �̵�
		{
			if (*x1 != (col - 1)) //(col-1)�̸� ������ ���������� �̵� ����
			{
				cursor1[*y1][*x1] = 0; //Ŀ������
				*x1 = *x1 + 1; //x1 ��ǥ ����
				cursor1[*y1][*x1] = 1; //�� ��ǥ�� Ŀ���ѱ�
			}
		}

		else if (input == ' ') //�����̽��ٰ� �Է�
		{
			if (*count_choice == 0) //ó�� ���õǴ°Ŷ��
				space(board, *x1, *y1, *count_turn, count_choice); //�̵��� ���� ����

			else //�̹� �̵��� ���� ���õ� ���¶��
			{
				for (i = 0; i < row; i++)
				{
					for (j = 0; j < col; j++)
					{
						if (board[i][j] == 3) //���� ���õ� ��
							x0 = j, y0 = i; //��ǥ ����
					}
				}

				if (test(board, *x1, *y1, *count_turn, x0, y0, &d) == 0) //������ ���ٸ�
				{
					if (d < 1.5) //sqrt2���� 1ĭ, ����
					{
						board[*y1][*x1] = *count_turn; //������ġ�� �÷��̾��� �������� ����
						board[y0][x0] = *count_turn; //���õǾ��ִ� ���� ���� �������
						score[1] += 1; //������ ��
					}

					else //2ĭ, �̵�
					{
						board[*y1][*x1] = *count_turn; //������ġ�� �÷��̾��� �������� ����
						board[y0][x0] = 0; //���õǾ��ִ� ������ �߸��������� �ǵ��ư�
					}

					for (i = 0; i < 3; i++) //������ǥ ���� 1ĭ���� x�� ����
						corx[i] = *x1 - 1 + i;

					for (i = 0; i < 3; i++) //������ǥ ���� 1ĭ���� y�� ����
						cory[i] = *y1 - 1 + i;

					for (i = 0; i < 3; i++)
					{
						for (j = 0; j < 3; j++)
						{
							if (corx[j] >= 0 && corx[j] <= col - 1 && cory[i] >= 0 && cory[i] <= row - 1) //��ǥ�� ������ ���� �ְ�
							{
								if (board[cory[i]][corx[j]] == 2) //��� �����̸�
								{
									board[cory[i]][corx[j]] = *count_turn; //���������� �ٲ���
									score[1] += 1; //������ �ø���
									score[2] -= 1; //������� ������
								}
							}
						}
					}

					*count_choice = 0; //ī���� �ʱ�ȭ
					*count_turn = *count_turn % 2 + 1; //�÷��̾� ��������
				}
			}
		}
	}

	else //�÷��̾� 2�� ��
	{
		if (input == 'i') //�����̵�
		{
			if (*y2 != 0) //0�̸� ������ ���� �̵� ����
			{
				cursor2[*y2][*x2] = 0; //Ŀ������
				*y2 = *y2 - 1; //y2 ��ǥ ����
				cursor2[*y2][*x2] = 1; //�� ��ǥ�� Ŀ���ѱ�
			}
		}

		else if (input == 'k') //�Ʒ����̵�
		{
			if (*y2 != (row - 1)) //(row-1)�̸� ������ �Ʒ��� �̵� ����
			{
				cursor2[*y2][*x2] = 0; //Ŀ������
				*y2 = *y2 + 1; //y2 ��ǥ ����
				cursor2[*y2][*x2] = 1; //�� ��ǥ�� Ŀ���ѱ� 
			}
		}

		else if (input == 'j') //�������� �̵�
		{
			if (*x2 != 0) //0�̸� ������ �������� �̵� ����
			{
				cursor2[*y2][*x2] = 0; //Ŀ������
				*x2 = *x2 - 1; //x2 ��ǥ ����
				cursor2[*y2][*x2] = 1; //�� ��ǥ�� Ŀ���ѱ�
			}
		}

		else if (input == 'l') //���������� �̵�
		{
			if (*x2 != (col - 1)) //(col-1)�̸� ������ ���������� �̵� ����
			{
				cursor2[*y2][*x2] = 0; //Ŀ������
				*x2 = *x2 + 1; //x2 ��ǥ ����
				cursor2[*y2][*x2] = 1; //�� ��ǥ�� Ŀ���ѱ�
			}
		}

		else if (input == ' ') //�����̽��ٰ� �Է�
		{
			if (*count_choice == 0) //ó�� ���õǴ°Ŷ��
				space(board, *x2, *y2, *count_turn, count_choice); //�̵��� ���� ����

			else //�̹� �̵��� ���� ���õ� ���¶��
			{
				for (i = 0; i < row; i++)
				{
					for (j = 0; j < col; j++)
					{
						if (board[i][j] == 3) //���� ���õ� ��
							x0 = j, y0 = i; //��ǥ ����
					}
				}

				if (test(board, *x2, *y2, *count_turn, x0, y0, &d) == 0) //������ ���ٸ�
				{
					if (d < 1.5) //sqrt2���� 1ĭ, ����
					{
						board[*y2][*x2] = *count_turn; //������ġ�� �÷��̾��� �������� ����
						board[y0][x0] = *count_turn; //���õǾ��ִ� ���� ���� �������
						score[2] += 1; //������ ��
					}

					else //2ĭ, �̵�
					{
						board[*y2][*x2] = *count_turn; //������ġ�� �÷��̾��� �������� ����
						board[y0][x0] = 0; //���õǾ��ִ� ������ �߸��������� �ǵ��ư�
					}

					for (i = 0; i < 3; i++) //������ǥ ���� 1ĭ���� x�� ����
						corx[i] = *x2 - 1 + i;

					for (i = 0; i < 3; i++) //������ǥ ���� 1ĭ���� y�� ����
						cory[i] = *y2 - 1 + i;

					for (i = 0; i < 3; i++)
					{
						for (j = 0; j < 3; j++)
						{
							if (corx[j] >= 0 && corx[j] <= col - 1 && cory[i] >= 0 && cory[i] <= row - 1) //��ǥ�� ������ ���� �ְ�
							{
								if (board[cory[i]][corx[j]] == 1) //��� �����̸�
								{
									board[cory[i]][corx[j]] = *count_turn; //���������� �ٲ���
									score[2] += 1; //������ �ø���
									score[1] -= 1; //������� ������
								}
							}
						}
					}

					*count_choice = 0; //ī���� �ʱ�ȭ
					*count_turn = *count_turn % 2 + 1; //�÷��̾� ��������
				}
			}
		}
	}
}

void gotoxy(int x, int y) //Ŀ�� �̵� �Լ�
{
	COORD pos = { x + 2 , y + 4 }; //(0,0)�� �������� �»�Ͱ� �ǵ��� ����
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); //Ŀ���̵�
}

void space(int** board, int x, int y, int count_turn, int* count_choice) //�� ���� �Լ�
{
	if (board[y][x] == count_turn) // �� �÷��̾��� ���� ������
	{
		board[y][x] = 3; //�� ����
		*count_choice = 1; //ī���� �ѱ�
	}
}

int test(int** board, int x, int y, int count_turn, int x0, int y0, float* d) //������ �������� �׽�Ʈ�ϴ� �Լ�
{
	if (board[y][x] == count_turn || board[y][x] == 3) //�÷��̾� �ڽ��� ������ ������ ���
	{
		board[y0][x0] = count_turn; //���
		board[y][x] = 3; //����� �缱��
		return 1; //������ �־����Ƿ� 1��ȯ
	}

	else if (board[y][x] == 0) //�󱸿��̸�
	{
		if (test_distance(x0, y0, x, y, d) == 0) //�̵� �����ϸ�
			return 0; //������ �����Ƿ� 0��ȯ
		
		else //�̵� �Ұ����ϸ�
			return 1; //������ �����Ƿ� 1��ȯ
	}

	else //�ٸ��÷��̾��� �����̸�
		return 1; //���������Ƿ� 1��ȯ
}

int test_distance(int x0, int y0, int x, int y, float* d) //�Ÿ� üũ �Լ�
{
	int dx = abs(x0 - x); //x��ǥ ���� �Ÿ� ����
	int dy = abs(y0 - y); //y��ǥ ���� �Ÿ� ����

	*d = (float)sqrt(pow(dx, 2) + pow(dy, 2)); //�Ÿ� ����

	if (dx == 0 && dy != 0)
	{
		if (dy <= 2) //�Ÿ��� 2�����̸�
			return 0; //�̵�����

		else //ũ��
			return 1; //�̵��Ұ���
	}

	else if (dx != 0 && dy == 0)
	{
		if (dx <= 2) //�Ÿ��� 2�����̸�
			return 0; //�̵�����

		else //ũ��
			return 1; //�̵��Ұ���
	}

	else if (dx != 0 && dy != 0)
	{
		if (*d < 3) //�ִ�Ÿ��� sqrt8�̹Ƿ� 3���� ������ �������
			return 0; //�̵�����

		else //ũ��
			return 1; //�̵��Ұ���
	}

	return 100; //������
}

int test_move(int** board, int* count_turn, int row, int col, int score[]) //�÷��̾��� ���� ������ ���� �ִ��� �׽�Ʈ�ϴ� �Լ�
{
	int i, j, x, y, count = 0, num = 0;
	int testx[5], testy[5];

	if (*count_turn == 1) //�÷��̾� 1�� ���� ���
	{
		for (i = 0; i < row; i++)
		{
			for (j = 0; j < col; j++)
			{
				if (board[i][j] == 1) //�÷��̾�1�� ������ ã����
				{
					for (x = 0; x < 5; x++) //������ǥ ���� 2ĭ ���� x�� ����
						testx[x] = j - 2 + x;

					for (y = 0; y < 5; y++) //���� ��ǥ ���� 2ĭ ���� y�� ����
						testy[y] = i - 2 + y;

					for (y = 0; y < 5; y++)
					{
						for (x = 0; x < 5; x++)
						{
							if (testx[x] >= 0 && testx[x] <= col - 1 && testy[y] >= 0 && testy[y] <= row - 1) //��ǥ�� ������ ���� �ִ� ���
							{
								if (board[testy[y]][testx[x]] != 0) //�� ������ �ƴϸ� 
									count += 1;
							}

							else //��ǥ�� ������ ���� ���� ���
								count += 1;
						}
					}

					if(count == 25) //�ֺ� 2ĭ���� ��翵������ �̵��� �� ����
						num += 1; //���� ������ ī��Ʈ
				}

				count = 0; //��Ȱ������ �ʱ�ȭ
			}
		}
		//������� �ϸ� num�� �̵��Ұ����� ���� ���� �����

		if (num == score[1]) //��� ���� �̵��Ұ����ϴٸ� (score�� ��� ���� ������ ����)
		{
			*count_turn = *count_turn % 2 + 1; //�÷��̾� ���� ����
			return 1; //Ư�����̽�
		}
	}

	else if (*count_turn == 2) //�÷��̾� 2�� ���� ���
	{
		for (i = 0; i < row; i++)
		{
			for (j = 0; j < col; j++)
			{
				if (board[i][j] == 2) //�÷��̾�2�� ������ ã����
				{
					for (x = 0; x < 5; x++) //������ǥ ���� 2ĭ ���� x�� ����
						testx[x] = j - 2 + x;

					for (y = 0; y < 5; y++) //���� ��ǥ ���� 2ĭ ���� y�� ����
						testy[y] = i - 2 + y;

					for (y = 0; y < 5; y++)
					{
						for (x = 0; x < 5; x++)
						{
							if (testx[x] >= 0 && testx[x] <= col - 1 && testy[y] >= 0 && testy[y] <= row - 1) //��ǥ�� ������ ���� �ִ� ���
							{
								if (board[testy[y]][testx[x]] != 0) //�� ������ �ƴϸ� 
									count += 1;
							}

							else //��ǥ�� ������ ���� ���� ���
								count += 1;
						}
					}

					if (count == 25) //�ֺ� 2ĭ���� ��翵������ �̵��� �� ����
						num += 1; //���� ������ ī��Ʈ
				}

				count = 0; //��Ȱ������ �ʱ�ȭ
			}
		}
		//������� �ϸ� num�� �̵��Ұ����� ���� ���� �����

		if (num == score[2]) //��� ���� �̵��Ұ����ϴٸ� (score�� ��� ���� ������ ����)
		{
			*count_turn = *count_turn % 2 + 1; //�÷��̾� ���� ����
			return 1; //Ư�����̽�
		}
	}

	return 0; //�������̽�
}