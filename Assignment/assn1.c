//개발환경은 visual studio입니다
# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int print_menu(); //초기메뉴화면 출력, 메뉴번호 입력받아 반환
void draw_line(); //50개의 "="출력(for문 사용) 
void menu_selection(int menu); //메뉴번호를 토대로 다음 작업으로 넘어간다 

void start_game(); //3회까지, 혹은 조기승리조건을 만족할때까지 게임을 진행한 후 결과를 출력하고 게임을 다시 할지 묻는다
int start_inning(int* earlyend, int inning, int* fscore1, int* fscore2, int* fscore3, int sum); //하나의 이닝 진행, 획득점수 반환
void print_score(int inning, int score1, int score2, int score3, int sum); //점수판 출력
void print_count(int ball, int strike, int out); //카운트 상황 출력
void print_base(int base1, int base2, int base3); //베이스 위의 주자 상황 출력
void end_check(int* out, int inning, int sum); //이닝 종료메시지 + 경기가 종료되었는지 체크 

int get_swing_result(); //스윙 결과 반환
void advance1(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3); //1루 진루
void advance2(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3); //2루 진루
void advance3(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3); //3루 진루
void homerun(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3); //홈런
void flyout(int* ball, int* strike, int* out); //플라이아웃
void swing_strike(int* ball, int* strike, int* out); //스트라이크

int get_pass_result(); //거르기 결과 반환
void pass_ball(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3); //볼

void print_info(); //게임 설명 출력
void menu_error(); //메뉴선택에서 잘못된 입력을 받은 경우

int main()
{
	int menu;
	srand((unsigned int)time(NULL)); //seed에 가변성 부여

	menu = print_menu(); //사용자가 입력한 메뉴 저장
	menu_selection(menu); //선택에 따라 진행 

	return 0;
}

int print_menu() //초기메뉴화면 출력, 메뉴번호 입력받아 반환
{
	int command; //사용자가 진행하고 싶은 메뉴

	draw_line(); //초기화면 위의 ======= 출력
	printf("\n"); //줄바꿈 
	printf("                  1. 게임 시작\n\n");
	printf("                  2. 게임 설명\n\n");
	printf("                  3. 게임 종료\n\n");
	draw_line(); //초기화면 아래의 ======= 출력
	printf("선택하고자 하는 메뉴를 입력하세요: ");
	//여기까지 메뉴 

	scanf("%d", &command); //메뉴번호 입력 받음 
	return command; //메뉴번호 반환 
}

void draw_line() //50개의 "="출력(for문 사용) 
{
	int i;

	for (i = 1; i <= 50; i++) //"=" 50개 출력 
		printf("=");

	printf("\n"); //줄바꿈 
}

void menu_selection(int menu)
{
	switch (menu) {
	case 1: start_game(); break; //게임시작
	case 2: print_info(); break; //설명출력
	case 3: printf("프로그램을 종료합니다...\n\n"); break; //게임종료
	default: menu_error(); //재입력요청
	}
}

void start_game() //3회까지, 혹은 조기승리조건을 만족할때까지 게임을 진행한 후 결과를 출력하고 게임을 다시 할지 묻는다
{
	int inning; //몇회인지를 나타내는 변수
	int fscore1 = 0, fscore2 = 0, fscore3 = 0; //x회의 최종점수
	int sum = 0; //총점
	int regame; //게임 다시 플레이 여부

	system("cls"); //화면지우기
	draw_line();
	printf("게임을 시작합니다.\n");
	draw_line();
	printf("\n");
	//여기까지 게임시작 안내문구

	for (inning = 1; inning <= 3; inning++) //이닝 시작(3회)
		sum = sum + start_inning(&inning, inning, &fscore1, &fscore2, &fscore3, sum); //총점 계산

	print_score(inning, fscore1, fscore2, fscore3, sum); //최종점수판 출력

	if (sum >= 10) //10점이상 승리
		printf("* 결과: 플레이어가 승리하였습니다.\n");
	else //10점이하 패배
		printf("* 결과:플레이어가 패배하였습니다.\n");

	printf("메인 메뉴로 돌아가기(-1): ");
	scanf("%d", &regame); //리플레이 여부 입력받음
	if (regame == -1) //리플레이 희망
	{
		system("cls");
		regame = print_menu(); //초기메뉴출력 및 메뉴번호 입력받아 regame에 저장
		menu_selection(regame); //게임메뉴 재출력
	}
}

int start_inning(int* earlyend, int inning, int* fscore1, int* fscore2, int* fscore3, int sum) //하나의 이닝 진행, 획득점수 반환
{
	int score1 = 0, score2 = 0, score3 = 0; //스윙 1회에서 획득한 점수
	int ball = 0, strike = 0, out = 0; //카운트 상황 변수
	int base1 = 0, base2 = 0, base3 = 0; //베이스 상황 변수 0==없음 1==있음
	int hit; //타자의 선택
	int result = 0; //스윙 또는 거르기의 결과

	//여기서부터 초기화면
	while (out <= 2) //아웃3번되면 한 이닝이 끝남
	{
		print_score(inning, *fscore1, *fscore2, *fscore3, sum); //점수판 출력
		print_count(ball, strike, out); //카운트상황 출력
		print_base(base1, base2, base3); //주자상황 출력
		printf("1. 스윙, 2. 거르기\n입력: ");
		scanf("%d", &hit); //타자가 칠지 말지 선택함
		
		system("cls"); // 화면 지우기

		if (hit == 1) //스윙
		{
			result = get_swing_result(); //스윙의 결과 대입

			switch (result) { //스윙의 결과에 따라 카운트값 변경 및 베이스 상황 변경
			case 1: advance1(inning, &ball, &strike, &base1, &base2, &base3, &score1, &score2, &score3); break; //1루타
			case 2: advance2(inning, &ball, &strike, &base1, &base2, &base3, &score1, &score2, &score3); break; //2루타
			case 3: advance3(inning, &ball, &strike, &base1, &base2, &base3, &score1, &score2, &score3); break; //3루타
			case 4: homerun(inning, &ball, &strike, &base1, &base2, &base3, &score1, &score2, &score3); break; //홈런
			case 5: flyout(&ball, &strike, &out); break; //플라이아웃
			case 6: swing_strike(&ball, &strike, &out); break; //스트라이크
			}
		}

		else if (hit == 2) //거르기
		{
			result = get_pass_result(); //거르기의 결과 대입

			switch (result) { //거르기의 결과에 따라 카운트값 변경 및 베이스 상황 변경
			case 1: pass_ball(inning, &ball, &strike, &base1, &base2, &base3, &score1, &score2, &score3); break; //볼 
			case 2: swing_strike(&ball, &strike, &out); break; //스트라이크 
			}
		}

		else //에러 메시지 출력
		{
			printf("숫자를 다시 입력하세요.\n");
		}

		*fscore1 = *fscore1 + score1; //획득한 점수만큼 실제 값에 저장
		*fscore2 = *fscore2 + score2;
		*fscore3 = *fscore3 + score3;
		score1 = 0; //획득한 스코어값 초기화
		score2 = 0;
		score3 = 0;

		sum = *fscore1 + *fscore2 + *fscore3; //조기종료 가능성 테스트 위해 합계 점수 계산
		end_check(&out, inning, sum);
		if (out == 3 && sum >= 10)
			*earlyend = 3; //start_game함수의 inning 값을 3으로 바꿔서 게임을 조기종료시킴(start_game에 돌아가서 for문이 끝나서 4가됨)
	}

	switch (inning) { //이닝별로 최종 스코어 반환
	case 1: return *fscore1;
	case 2: return *fscore2;
	case 3: return *fscore3;
	default: return 0;
	}
}

void print_score(int inning, int score1, int score2, int score3, int sum) //점수판 출력
{
	if (inning <= 3)
	{
		draw_line(); //============줄생김
		printf("%d회\n", inning); //현재이닝
	}

	printf("┌───────────────────────────────────────────┐\n");
	printf("│            1회   2회   3회      총 점     │\n");
	switch (inning) {
	case 1: printf("  점수         %d\n", score1); break;
	case 2: printf("  점수         %d    %d\n", score1, score2); break;
	case 3: printf("  점수         %d    %d    %d\n", score1, score2, score3); break;
	case 4: printf("  점수         %d    %d    %d        %d\n", score1, score2, score3, sum); break;
	}
	printf("│                                           │\n");
	printf("└───────────────────────────────────────────┘\n");
}

void print_count(int ball, int strike, int out) //카운트 상황 출력
{
	switch (ball) { //볼 상황 출력
	case 0: printf("B ○○○\n"); break;
	case 1: printf("B ●○○\n"); break;
	case 2: printf("B ●●○\n"); break;
	case 3: printf("B ●●●\n"); break;
	}
	switch (strike) { //스트라이크 상황 출력
	case 0: printf("S ○○\n"); break;
	case 1: printf("S ●○\n"); break;
	case 2: printf("S ●●\n"); break;
	}
	switch (out) { //아웃 상황 출력
	case 0: printf("O ○○\n"); break;
	case 1: printf("O ●○\n"); break;
	case 2: printf("O ●●\n"); break;
	}
}

void print_base(int base1, int base2, int base3) //베이스 위의 주자 상황 출력
{
	if (base2 == 0) //2루 없음
		printf("                      ◇\n");
	else //2루 있음
		printf("                      ◆\n");

	printf("                    ↙  ↖\n");
	printf("                  ↙      ↖\n");
	printf("                ↙          ↖\n");

	if (base1 == 0 && base3 == 0) //1루, 3루 전부 없음
		printf("              ◇              ◇\n");
	else if (base1 == 0 && base3 == 1) //1루 없고, 3루 있음
		printf("              ◆              ◇\n");
	else if (base1 == 1 && base3 == 0) //1루 있고, 3루 없음
		printf("              ◇              ◆\n");
	else //1루, 3루 전부 있음
		printf("              ◆              ◆\n");

	printf("                ↘          ↗\n");
	printf("                  ↘      ↗\n");
	printf("                    ↘  ↗\n");
	printf("                      ◇\n");
}

void end_check(int* out, int inning, int sum) //이닝 종료메시지 + 경기가 종료되었는지 체크 
{
	if (*out == 3)
		printf("이닝이 종료되었습니다.\n");

	if (*out == 3 && sum >= 10) //점수가 10점이 넘어서 종료 
	{
		printf("\n경기가 종료되었습니다.\n");
		draw_line();
		printf("\n");
	}
	else if (*out == 3 && inning == 3) //3회에 3아웃으로 경기 종료(10점 안넘는 경우) 
	{	
		printf("\n경기가 종료되었습니다.\n");
		draw_line();
		printf("\n");
	}
}

int get_swing_result() //스윙 결과 반환
{
	int result; //스윙의 결과
	result = rand() % 100 + 1; //숫자범위 1-100

	if (result <= 14) //1루타 14%
		return 1;
	else if (result <= 21) //2루타 7%
		return 2;
	else if (result <= 26) //3루타 5%
		return 3;
	else if (result <= 29) //홈런 3%
		return 4;
	else if (result <= 62) //플라이 아웃 33%
		return 5;
	else //스트라이크 38%
		return 6;
}

void advance1(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3) //1루 진루
{
	*ball = 0, * strike = 0; //BS초기화
	printf("안타입니다.\n");

	if (*base3 == 1) //3루->홈
	{
		printf("\n홈인! 점수를 한 점 올립니다.\n");
		switch (inning) {
		case 1: *score1 = *score1 + 1; break;
		case 2: *score2 = *score2 + 1; break;
		case 3: *score3 = *score3 + 1; break;
		}
		*base3 = 0;
	}

	if (*base2 == 1) //2루->3루
	{
		*base3 = 1;
		*base2 = 0;
	}

	if (*base1 == 1) //1루->2루
	{
		*base2 = 1;
		*base1 = 0;
	}
	*base1 = 1; //홈->1루(타자)
}

void advance2(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3) //2루 진루
{
	*ball = 0, * strike = 0; //BS초기화
	printf("2루타입니다.\n");

	if (*base3 == 1) //3루->홈
	{
		printf("\n3루 주자 홈인! 점수를 한 점 올립니다.\n");
		switch (inning) {
		case 1: *score1 = *score1 + 1; break;
		case 2: *score2 = *score2 + 1; break;
		case 3: *score3 = *score3 + 1; break;
		}
		*base3 = 0;
	}

	if (*base2 == 1) //2루->홈
	{
		printf("\n2루 주자 홈인! 점수를 한 점 올립니다.\n");
		switch (inning) {
		case 1: *score1 = *score1 + 1; break;
		case 2: *score2 = *score2 + 1; break;
		case 3: *score3 = *score3 + 1; break;
		}
		*base2 = 0;
	}

	if (*base1 == 1) //1루->3루
	{
		*base3 = 1;
		*base1 = 0;
	}
	*base2 = 1; //홈->2루(타자)
}

void advance3(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3) //3루 진루
{
	*ball = 0, * strike = 0; //BS초기화
	printf("3루타입니다.\n");

	if (*base3 == 1) //3루->홈
	{
		printf("\n3루 주자 홈인! 점수를 한 점 올립니다.\n");
		switch (inning) {
		case 1: *score1 = *score1 + 1; break;
		case 2: *score2 = *score2 + 1; break;
		case 3: *score3 = *score3 + 1; break;
		}
		*base3 = 0;
	}

	if (*base2 == 1) //2루->홈
	{
		printf("\n2루 주자 홈인! 점수를 한 점 올립니다.\n");
		switch (inning) {
		case 1: *score1 = *score1 + 1; break;
		case 2: *score2 = *score2 + 1; break;
		case 3: *score3 = *score3 + 1; break;
		}
		*base2 = 0;
	}

	if (*base1 == 1) //1루->홈
	{
		printf("\n1루 주자 홈인! 점수를 한 점 올립니다.\n");
		switch (inning) {
		case 1: *score1 = *score1 + 1; break;
		case 2: *score2 = *score2 + 1; break;
		case 3: *score3 = *score3 + 1; break;
		}
		*base1 = 0;
	}
	*base3 = 1; //홈->3루(타자)
}

void homerun(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3) //홈런
{
	*ball = 0, * strike = 0; //BS초기화
	printf("홈런입니다.\n");

	if (*base3 == 1) //3루->홈
	{
		printf("\n3루 주자 홈인! 점수를 한 점 올립니다.\n");
		switch (inning) {
		case 1: *score1 = *score1 + 1; break;
		case 2: *score2 = *score2 + 1; break;
		case 3: *score3 = *score3 + 1; break;
		}
		*base3 = 0;
	}

	if (*base2 == 1) //2루->홈
	{
		printf("\n2루 주자 홈인! 점수를 한 점 올립니다.\n");
		switch (inning) {
		case 1: *score1 = *score1 + 1; break;
		case 2: *score2 = *score2 + 1; break;
		case 3: *score3 = *score3 + 1; break;
		}
		*base2 = 0;
	}

	if (*base1 == 1) //1루->홈
	{
		printf("\n1루 주자 홈인! 점수를 한 점 올립니다.\n");
		switch (inning) {
		case 1: *score1 = *score1 + 1; break;
		case 2: *score2 = *score2 + 1; break;
		case 3: *score3 = *score3 + 1; break;
		}
		*base1 = 0;
	}

	printf("\n타자 홈인! 점수를 한 점 올립니다.\n"); //타자->홈
	switch (inning) {
	case 1: *score1 = *score1 + 1; break;
	case 2: *score2 = *score2 + 1; break;
	case 3: *score3 = *score3 + 1; break;
	}
}

void flyout(int* ball, int* strike, int* out) //플라이아웃
{
	*out = *out + 1; //아웃카운트 증가
	*ball = 0;
	*strike = 0; //BS초기화

	printf("플라이 아웃입니다.\n");
}

void swing_strike(int* ball, int* strike, int* out) //스트라이크
{
	*strike = *strike + 1; //스트라이크 카운트 +1

	if (*strike <= 2)
		printf("스트라이크 입니다.\n");

	else //스트라이크 아웃인경우
	{
		*ball = 0;
		*strike = 0;
		*out = *out + 1; //BS초기화 및 아웃 카운트 +1

		printf("스트라이크 아웃!!\n");
	}
}

int get_pass_result() //거르기 결과 반환
{
	int result; //거르기의 결과
	result = rand() % 100 + 1; //숫자범위 1-100

	if (result <= 65) //볼 65%
		return 1;
	else //스트라이크 35%
		return 2;
}

void pass_ball(int inning, int* ball, int* strike, int* base1, int* base2, int* base3, int* score1, int* score2, int* score3) //볼
{
	*ball = *ball + 1;

	if (*ball <= 3) //볼넷이 아닌 볼인 경우
		printf("볼입니다.\n");

	else  //볼넷
	{
		printf("볼넷입니다.\n");

		if (*base1 == 1) //1루에 주자가 있을 때
		{
			if (*base2 == 1) //1루, 2루에 주자 있을 때
			{
				if (*base3 == 1) //1루, 2루, 3루에 주자 있을 때
				{
					printf("\n홈인! 점수를 한 점 올립니다.\n");

					switch (inning) { //점수 +1점
					case 1: *score1 = *score1 + 1; break;
					case 2: *score2 = *score2 + 1; break;
					case 3: *score3 = *score3 + 1; break;
					}
				}
				else //1루, 2루에 주자 있고 3루에 주자 없을 때
					*base3 = 1; //타자와 두명의 주자가 1루씩 이동
			}
			else //1루에 주자 있고, 2루에 주자 없을 때
				*base2 = 1; //3루의 주자는 의미없음, 1,2 루에 주자가 모두 존재하게 될 것
		}
		else //1루에 주자가 없을 때
			*base1 = 1; //홈->1루 이동(타자)

		*ball = 0;
		*strike = 0; //BS초기화
	}
}

void print_info() //게임 설명 출력
{
	int m; //메인메뉴 관련 변수
	system("cls"); //화면 지우기

	printf("---------------------------미니 야구 게임 설명---------------------------\n\n");
	printf("본 게임은 야구의 룰을 일부 적용한 '미니 야구 게임' 입니다.\n\n");
	printf("  1. 게임은 총 3회로 이루어져 있습니다.\n\n");
	printf("  2. 플레이어는 스윙 혹은 공을 거를 수 있습니다.\n\n");
	printf("  3. 안타를 치거나 볼넷이 되면 출루 할 수 있습니다.\n\n");
	printf("  4. 주자에 의한 아웃은 없으며, 플라이 아웃, 스트라이크 아웃만 있습니다.\n\n");
	printf("  5. 3회까지 득점한 점수가 10점 이상이면 게임에서 승리하게 됩니다.\n");
	printf("-------------------------------------------------------------------------\n");

	printf("메인 메뉴로 돌아가기(-1): ");
	scanf("%d", &m);

	if (m == -1) //메인메뉴로 돌아간다.
	{
		system("cls");
		m = print_menu();
		menu_selection(m);
	}
}

void menu_error() //메뉴선택에서 잘못된 입력을 받은 경우
{
	int menu; //메뉴
	system("cls");
	printf("메뉴를 다시 선택해주세요.\n");
	menu = print_menu();
	menu_selection(menu);
}