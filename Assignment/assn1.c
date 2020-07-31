//����ȯ���� visual studio�Դϴ�
# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int print_menu(); //�ʱ�޴�ȭ�� ���, �޴���ȣ �Է¹޾� ��ȯ
void draw_line(); //50���� "="���(for�� ���) 
void menu_selection(int menu); //�޴���ȣ�� ���� ���� �۾����� �Ѿ�� 

void start_game(); //3ȸ����, Ȥ�� ����¸������� �����Ҷ����� ������ ������ �� ����� ����ϰ� ������ �ٽ� ���� ���´�
int start_inning(int* earlyend, int inning, int* fscore1, int* fscore2, int* fscore3, int sum); //�ϳ��� �̴� ����, ȹ������ ��ȯ
void print_score(int inning, int score1, int score2, int score3, int sum); //������ ���
void print_count(int ball, int strike, int out); //ī��Ʈ ��Ȳ ���
void print_base(int base1, int base2, int base3); //���̽� ���� ���� ��Ȳ ���
void end_check(int* out, int inning, int sum); //�̴� ����޽��� + ��Ⱑ ����Ǿ����� üũ 

int get_swing_result(); //���� ��� ��ȯ
void advance1(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3); //1�� ����
void advance2(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3); //2�� ����
void advance3(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3); //3�� ����
void homerun(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3); //Ȩ��
void flyout(int* ball, int* strike, int* out); //�ö��̾ƿ�
void swing_strike(int* ball, int* strike, int* out); //��Ʈ����ũ

int get_pass_result(); //�Ÿ��� ��� ��ȯ
void pass_ball(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3); //��

void print_info(); //���� ���� ���
void menu_error(); //�޴����ÿ��� �߸��� �Է��� ���� ���

int main()
{
	int menu;
	srand((unsigned int)time(NULL)); //seed�� ������ �ο�

	menu = print_menu(); //����ڰ� �Է��� �޴� ����
	menu_selection(menu); //���ÿ� ���� ���� 

	return 0;
}

int print_menu() //�ʱ�޴�ȭ�� ���, �޴���ȣ �Է¹޾� ��ȯ
{
	int command; //����ڰ� �����ϰ� ���� �޴�

	draw_line(); //�ʱ�ȭ�� ���� ======= ���
	printf("\n"); //�ٹٲ� 
	printf("                  1. ���� ����\n\n");
	printf("                  2. ���� ����\n\n");
	printf("                  3. ���� ����\n\n");
	draw_line(); //�ʱ�ȭ�� �Ʒ��� ======= ���
	printf("�����ϰ��� �ϴ� �޴��� �Է��ϼ���: ");
	//������� �޴� 

	scanf("%d", &command); //�޴���ȣ �Է� ���� 
	return command; //�޴���ȣ ��ȯ 
}

void draw_line() //50���� "="���(for�� ���) 
{
	int i;

	for (i = 1; i <= 50; i++) //"=" 50�� ��� 
		printf("=");

	printf("\n"); //�ٹٲ� 
}

void menu_selection(int menu)
{
	switch (menu) {
	case 1: start_game(); break; //���ӽ���
	case 2: print_info(); break; //�������
	case 3: printf("���α׷��� �����մϴ�...\n\n"); break; //��������
	default: menu_error(); //���Է¿�û
	}
}

void start_game() //3ȸ����, Ȥ�� ����¸������� �����Ҷ����� ������ ������ �� ����� ����ϰ� ������ �ٽ� ���� ���´�
{
	int inning; //��ȸ������ ��Ÿ���� ����
	int fscore1 = 0, fscore2 = 0, fscore3 = 0; //xȸ�� ��������
	int sum = 0; //����
	int regame; //���� �ٽ� �÷��� ����

	system("cls"); //ȭ�������
	draw_line();
	printf("������ �����մϴ�.\n");
	draw_line();
	printf("\n");
	//������� ���ӽ��� �ȳ�����

	for (inning = 1; inning <= 3; inning++) //�̴� ����(3ȸ)
		sum = sum + start_inning(&inning, inning, &fscore1, &fscore2, &fscore3, sum); //���� ���

	print_score(inning, fscore1, fscore2, fscore3, sum); //���������� ���

	if (sum >= 10) //10���̻� �¸�
		printf("* ���: �÷��̾ �¸��Ͽ����ϴ�.\n");
	else //10������ �й�
		printf("* ���:�÷��̾ �й��Ͽ����ϴ�.\n");

	printf("���� �޴��� ���ư���(-1): ");
	scanf("%d", &regame); //���÷��� ���� �Է¹���
	if (regame == -1) //���÷��� ���
	{
		system("cls");
		regame = print_menu(); //�ʱ�޴���� �� �޴���ȣ �Է¹޾� regame�� ����
		menu_selection(regame); //���Ӹ޴� �����
	}
}

int start_inning(int* earlyend, int inning, int* fscore1, int* fscore2, int* fscore3, int sum) //�ϳ��� �̴� ����, ȹ������ ��ȯ
{
	int score1 = 0, score2 = 0, score3 = 0; //���� 1ȸ���� ȹ���� ����
	int ball = 0, strike = 0, out = 0; //ī��Ʈ ��Ȳ ����
	int base1 = 0, base2 = 0, base3 = 0; //���̽� ��Ȳ ���� 0==���� 1==����
	int hit; //Ÿ���� ����
	int result = 0; //���� �Ǵ� �Ÿ����� ���

	//���⼭���� �ʱ�ȭ��
	while (out <= 2) //�ƿ�3���Ǹ� �� �̴��� ����
	{
		print_score(inning, *fscore1, *fscore2, *fscore3, sum); //������ ���
		print_count(ball, strike, out); //ī��Ʈ��Ȳ ���
		print_base(base1, base2, base3); //���ڻ�Ȳ ���
		printf("1. ����, 2. �Ÿ���\n�Է�: ");
		scanf("%d", &hit); //Ÿ�ڰ� ĥ�� ���� ������
		
		system("cls"); // ȭ�� �����

		if (hit == 1) //����
		{
			result = get_swing_result(); //������ ��� ����

			switch (result) { //������ ����� ���� ī��Ʈ�� ���� �� ���̽� ��Ȳ ����
			case 1: advance1(inning, &ball, &strike, &base1, &base2, &base3, &score1, &score2, &score3); break; //1��Ÿ
			case 2: advance2(inning, &ball, &strike, &base1, &base2, &base3, &score1, &score2, &score3); break; //2��Ÿ
			case 3: advance3(inning, &ball, &strike, &base1, &base2, &base3, &score1, &score2, &score3); break; //3��Ÿ
			case 4: homerun(inning, &ball, &strike, &base1, &base2, &base3, &score1, &score2, &score3); break; //Ȩ��
			case 5: flyout(&ball, &strike, &out); break; //�ö��̾ƿ�
			case 6: swing_strike(&ball, &strike, &out); break; //��Ʈ����ũ
			}
		}

		else if (hit == 2) //�Ÿ���
		{
			result = get_pass_result(); //�Ÿ����� ��� ����

			switch (result) { //�Ÿ����� ����� ���� ī��Ʈ�� ���� �� ���̽� ��Ȳ ����
			case 1: pass_ball(inning, &ball, &strike, &base1, &base2, &base3, &score1, &score2, &score3); break; //�� 
			case 2: swing_strike(&ball, &strike, &out); break; //��Ʈ����ũ 
			}
		}

		else //���� �޽��� ���
		{
			printf("���ڸ� �ٽ� �Է��ϼ���.\n");
		}

		*fscore1 = *fscore1 + score1; //ȹ���� ������ŭ ���� ���� ����
		*fscore2 = *fscore2 + score2;
		*fscore3 = *fscore3 + score3;
		score1 = 0; //ȹ���� ���ھ �ʱ�ȭ
		score2 = 0;
		score3 = 0;

		sum = *fscore1 + *fscore2 + *fscore3; //�������� ���ɼ� �׽�Ʈ ���� �հ� ���� ���
		end_check(&out, inning, sum);
		if (out == 3 && sum >= 10)
			*earlyend = 3; //start_game�Լ��� inning ���� 3���� �ٲ㼭 ������ ���������Ŵ(start_game�� ���ư��� for���� ������ 4����)
	}

	switch (inning) { //�̴׺��� ���� ���ھ� ��ȯ
	case 1: return *fscore1;
	case 2: return *fscore2;
	case 3: return *fscore3;
	default: return 0;
	}
}

void print_score(int inning, int score1, int score2, int score3, int sum) //������ ���
{
	if (inning <= 3)
	{
		draw_line(); //============�ٻ���
		printf("%dȸ\n", inning); //�����̴�
	}

	printf("������������������������������������������������������������������������������������������\n");
	printf("��            1ȸ   2ȸ   3ȸ      �� ��     ��\n");
	switch (inning) {
	case 1: printf("  ����         %d\n", score1); break;
	case 2: printf("  ����         %d    %d\n", score1, score2); break;
	case 3: printf("  ����         %d    %d    %d\n", score1, score2, score3); break;
	case 4: printf("  ����         %d    %d    %d        %d\n", score1, score2, score3, sum); break;
	}
	printf("��                                           ��\n");
	printf("������������������������������������������������������������������������������������������\n");
}

void print_count(int ball, int strike, int out) //ī��Ʈ ��Ȳ ���
{
	switch (ball) { //�� ��Ȳ ���
	case 0: printf("B �ۡۡ�\n"); break;
	case 1: printf("B �ܡۡ�\n"); break;
	case 2: printf("B �ܡܡ�\n"); break;
	case 3: printf("B �ܡܡ�\n"); break;
	}
	switch (strike) { //��Ʈ����ũ ��Ȳ ���
	case 0: printf("S �ۡ�\n"); break;
	case 1: printf("S �ܡ�\n"); break;
	case 2: printf("S �ܡ�\n"); break;
	}
	switch (out) { //�ƿ� ��Ȳ ���
	case 0: printf("O �ۡ�\n"); break;
	case 1: printf("O �ܡ�\n"); break;
	case 2: printf("O �ܡ�\n"); break;
	}
}

void print_base(int base1, int base2, int base3) //���̽� ���� ���� ��Ȳ ���
{
	if (base2 == 0) //2�� ����
		printf("                      ��\n");
	else //2�� ����
		printf("                      ��\n");

	printf("                    ��  ��\n");
	printf("                  ��      ��\n");
	printf("                ��          ��\n");

	if (base1 == 0 && base3 == 0) //1��, 3�� ���� ����
		printf("              ��              ��\n");
	else if (base1 == 0 && base3 == 1) //1�� ����, 3�� ����
		printf("              ��              ��\n");
	else if (base1 == 1 && base3 == 0) //1�� �ְ�, 3�� ����
		printf("              ��              ��\n");
	else //1��, 3�� ���� ����
		printf("              ��              ��\n");

	printf("                ��          ��\n");
	printf("                  ��      ��\n");
	printf("                    ��  ��\n");
	printf("                      ��\n");
}

void end_check(int* out, int inning, int sum) //�̴� ����޽��� + ��Ⱑ ����Ǿ����� üũ 
{
	if (*out == 3)
		printf("�̴��� ����Ǿ����ϴ�.\n");

	if (*out == 3 && sum >= 10) //������ 10���� �Ѿ ���� 
	{
		printf("\n��Ⱑ ����Ǿ����ϴ�.\n");
		draw_line();
		printf("\n");
	}
	else if (*out == 3 && inning == 3) //3ȸ�� 3�ƿ����� ��� ����(10�� �ȳѴ� ���) 
	{	
		printf("\n��Ⱑ ����Ǿ����ϴ�.\n");
		draw_line();
		printf("\n");
	}
}

int get_swing_result() //���� ��� ��ȯ
{
	int result; //������ ���
	result = rand() % 100 + 1; //���ڹ��� 1-100

	if (result <= 14) //1��Ÿ 14%
		return 1;
	else if (result <= 21) //2��Ÿ 7%
		return 2;
	else if (result <= 26) //3��Ÿ 5%
		return 3;
	else if (result <= 29) //Ȩ�� 3%
		return 4;
	else if (result <= 62) //�ö��� �ƿ� 33%
		return 5;
	else //��Ʈ����ũ 38%
		return 6;
}

void advance1(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3) //1�� ����
{
	*ball = 0, * strike = 0; //BS�ʱ�ȭ
	printf("��Ÿ�Դϴ�.\n");

	if (*base3 == 1) //3��->Ȩ
	{
		printf("\nȨ��! ������ �� �� �ø��ϴ�.\n");
		switch (inning) {
		case 1: *score1 = *score1 + 1; break;
		case 2: *score2 = *score2 + 1; break;
		case 3: *score3 = *score3 + 1; break;
		}
		*base3 = 0;
	}

	if (*base2 == 1) //2��->3��
	{
		*base3 = 1;
		*base2 = 0;
	}

	if (*base1 == 1) //1��->2��
	{
		*base2 = 1;
		*base1 = 0;
	}
	*base1 = 1; //Ȩ->1��(Ÿ��)
}

void advance2(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3) //2�� ����
{
	*ball = 0, * strike = 0; //BS�ʱ�ȭ
	printf("2��Ÿ�Դϴ�.\n");

	if (*base3 == 1) //3��->Ȩ
	{
		printf("\n3�� ���� Ȩ��! ������ �� �� �ø��ϴ�.\n");
		switch (inning) {
		case 1: *score1 = *score1 + 1; break;
		case 2: *score2 = *score2 + 1; break;
		case 3: *score3 = *score3 + 1; break;
		}
		*base3 = 0;
	}

	if (*base2 == 1) //2��->Ȩ
	{
		printf("\n2�� ���� Ȩ��! ������ �� �� �ø��ϴ�.\n");
		switch (inning) {
		case 1: *score1 = *score1 + 1; break;
		case 2: *score2 = *score2 + 1; break;
		case 3: *score3 = *score3 + 1; break;
		}
		*base2 = 0;
	}

	if (*base1 == 1) //1��->3��
	{
		*base3 = 1;
		*base1 = 0;
	}
	*base2 = 1; //Ȩ->2��(Ÿ��)
}

void advance3(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3) //3�� ����
{
	*ball = 0, * strike = 0; //BS�ʱ�ȭ
	printf("3��Ÿ�Դϴ�.\n");

	if (*base3 == 1) //3��->Ȩ
	{
		printf("\n3�� ���� Ȩ��! ������ �� �� �ø��ϴ�.\n");
		switch (inning) {
		case 1: *score1 = *score1 + 1; break;
		case 2: *score2 = *score2 + 1; break;
		case 3: *score3 = *score3 + 1; break;
		}
		*base3 = 0;
	}

	if (*base2 == 1) //2��->Ȩ
	{
		printf("\n2�� ���� Ȩ��! ������ �� �� �ø��ϴ�.\n");
		switch (inning) {
		case 1: *score1 = *score1 + 1; break;
		case 2: *score2 = *score2 + 1; break;
		case 3: *score3 = *score3 + 1; break;
		}
		*base2 = 0;
	}

	if (*base1 == 1) //1��->Ȩ
	{
		printf("\n1�� ���� Ȩ��! ������ �� �� �ø��ϴ�.\n");
		switch (inning) {
		case 1: *score1 = *score1 + 1; break;
		case 2: *score2 = *score2 + 1; break;
		case 3: *score3 = *score3 + 1; break;
		}
		*base1 = 0;
	}
	*base3 = 1; //Ȩ->3��(Ÿ��)
}

void homerun(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3) //Ȩ��
{
	*ball = 0, * strike = 0; //BS�ʱ�ȭ
	printf("Ȩ���Դϴ�.\n");

	if (*base3 == 1) //3��->Ȩ
	{
		printf("\n3�� ���� Ȩ��! ������ �� �� �ø��ϴ�.\n");
		switch (inning) {
		case 1: *score1 = *score1 + 1; break;
		case 2: *score2 = *score2 + 1; break;
		case 3: *score3 = *score3 + 1; break;
		}
		*base3 = 0;
	}

	if (*base2 == 1) //2��->Ȩ
	{
		printf("\n2�� ���� Ȩ��! ������ �� �� �ø��ϴ�.\n");
		switch (inning) {
		case 1: *score1 = *score1 + 1; break;
		case 2: *score2 = *score2 + 1; break;
		case 3: *score3 = *score3 + 1; break;
		}
		*base2 = 0;
	}

	if (*base1 == 1) //1��->Ȩ
	{
		printf("\n1�� ���� Ȩ��! ������ �� �� �ø��ϴ�.\n");
		switch (inning) {
		case 1: *score1 = *score1 + 1; break;
		case 2: *score2 = *score2 + 1; break;
		case 3: *score3 = *score3 + 1; break;
		}
		*base1 = 0;
	}

	printf("\nŸ�� Ȩ��! ������ �� �� �ø��ϴ�.\n"); //Ÿ��->Ȩ
	switch (inning) {
	case 1: *score1 = *score1 + 1; break;
	case 2: *score2 = *score2 + 1; break;
	case 3: *score3 = *score3 + 1; break;
	}
}

void flyout(int* ball, int* strike, int* out) //�ö��̾ƿ�
{
	*out = *out + 1; //�ƿ�ī��Ʈ ����
	*ball = 0;
	*strike = 0; //BS�ʱ�ȭ

	printf("�ö��� �ƿ��Դϴ�.\n");
}

void swing_strike(int* ball, int* strike, int* out) //��Ʈ����ũ
{
	*strike = *strike + 1; //��Ʈ����ũ ī��Ʈ +1

	if (*strike <= 2)
		printf("��Ʈ����ũ �Դϴ�.\n");

	else //��Ʈ����ũ �ƿ��ΰ��
	{
		*ball = 0;
		*strike = 0;
		*out = *out + 1; //BS�ʱ�ȭ �� �ƿ� ī��Ʈ +1

		printf("��Ʈ����ũ �ƿ�!!\n");
	}
}

int get_pass_result() //�Ÿ��� ��� ��ȯ
{
	int result; //�Ÿ����� ���
	result = rand() % 100 + 1; //���ڹ��� 1-100

	if (result <= 65) //�� 65%
		return 1;
	else //��Ʈ����ũ 35%
		return 2;
}

void pass_ball(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3) //��
{
	*ball = *ball + 1;

	if (*ball <= 3) //������ �ƴ� ���� ���
		printf("���Դϴ�.\n");

	else  //����
	{
		printf("�����Դϴ�.\n");

		if (*base1 == 1) //1�翡 ���ڰ� ���� ��
		{
			if (*base2 == 1) //1��, 2�翡 ���� ���� ��
			{
				if (*base3 == 1) //1��, 2��, 3�翡 ���� ���� ��
				{
					printf("\nȨ��! ������ �� �� �ø��ϴ�.\n");

					switch (inning) { //���� +1��
					case 1: *score1 = *score1 + 1; break;
					case 2: *score2 = *score2 + 1; break;
					case 3: *score3 = *score3 + 1; break;
					}
				}
				else //1��, 2�翡 ���� �ְ� 3�翡 ���� ���� ��
					*base3 = 1; //Ÿ�ڿ� �θ��� ���ڰ� 1�羿 �̵�
			}
			else //1�翡 ���� �ְ�, 2�翡 ���� ���� ��
				*base2 = 1; //3���� ���ڴ� �ǹ̾���, 1,2 �翡 ���ڰ� ��� �����ϰ� �� ��
		}
		else //1�翡 ���ڰ� ���� ��
			*base1 = 1; //Ȩ->1�� �̵�(Ÿ��)

		*ball = 0;
		*strike = 0; //BS�ʱ�ȭ
	}
}

void print_info() //���� ���� ���
{
	int m; //���θ޴� ���� ����
	system("cls"); //ȭ�� �����

	printf("---------------------------�̴� �߱� ���� ����---------------------------\n\n");
	printf("�� ������ �߱��� ���� �Ϻ� ������ '�̴� �߱� ����' �Դϴ�.\n\n");
	printf("  1. ������ �� 3ȸ�� �̷���� �ֽ��ϴ�.\n\n");
	printf("  2. �÷��̾�� ���� Ȥ�� ���� �Ÿ� �� �ֽ��ϴ�.\n\n");
	printf("  3. ��Ÿ�� ġ�ų� ������ �Ǹ� ��� �� �� �ֽ��ϴ�.\n\n");
	printf("  4. ���ڿ� ���� �ƿ��� ������, �ö��� �ƿ�, ��Ʈ����ũ �ƿ��� �ֽ��ϴ�.\n\n");
	printf("  5. 3ȸ���� ������ ������ 10�� �̻��̸� ���ӿ��� �¸��ϰ� �˴ϴ�.\n");
	printf("-------------------------------------------------------------------------\n");

	printf("���� �޴��� ���ư���(-1): ");
	scanf("%d", &m);

	if (m == -1) //���θ޴��� ���ư���.
	{
		system("cls");
		m = print_menu();
		menu_selection(m);
	}
}

void menu_error() //�޴����ÿ��� �߸��� �Է��� ���� ���
{
	int menu; //�޴�
	system("cls");
	printf("�޴��� �ٽ� �������ּ���.\n");
	menu = print_menu();
	menu_selection(menu);
}