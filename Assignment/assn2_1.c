#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define EMAIL_MAX 30 //이메일 최대길이
#define PW_MAX 30 //비밀번호 최대길이

void make_password(char id[], char domain[], char pw_real[]); //이메일로부터 패스워드 생성하는 함수
int validate_email(char email[], char pw[]); //이메일형식일치, 허용도메인인지 체크하는 함수

void scan(char email[], char pw[]); //이메일, 비밀번호 입력받음
int pw_check(char email[], char pw[], int start_domain); //입력받은 비밀번호와 실제 비밀번호를 비교함

void result(int res); //결과출력

int strcmp(const char s1[], const char s2[]); //문자열 비교
int strlen(const char str[]); //문자열 길이 반환
void strcpy(char copyed[], char original[]); //문자열 복사

int main()
{
	int res;
	char re;
	char email[EMAIL_MAX+1]; //이메일이 30자가 입력되는경우 '\0'이 들어갈 공간이 있어야 하므로 크기는 최대길이 +1로 설정한다
	char pw[PW_MAX+1]; //비밀번호도 마찬가지

	while (1)
	{
		printf("---LOGIN TO ACCESS HEA SYSTEM---\n\n");
		scan(email, pw); //입력받음
		res = validate_email(email, pw); //인증성공여부 확인목적

		if (res != 1) //인증실패했으면
		{
			printf("\nDo you want try again? (y/n) ");
			scanf(" %c", &re); //재입력의사 물어봄, " %c"를 사용하여 버퍼에 남아있던 개행문자 무시

			if (re == 'y')
				printf("\n"); //줄바꿔주고 처음부터 다시 시작

			else if (re == 'n') //n이 입력되면 종료
				break; //프로그램을 종료하기 위해 반복문 탈출
		}

		else break; //비밀번호가 정확하면 프로그램 종료해야하므로 반복문 탈출
	}
	return 0;
}

void make_password(char id[], char domain[], char pw_real[]) //이메일로부터 real password 생성
{
	int i, len_id, len_domain, id_count = 0, domain_count = 0;

	len_id = strlen(id); //id의 길이가 저장된다
	len_domain = strlen(domain); //domain의 길이가 저장된다

	if (len_id == len_domain) //id, domain 길이 같은 경우
	{
		for (i = 0; i < 10 && domain[domain_count] != '\0'; i++) //10개를 채우거나 id, domain의 문자들을 다 사용할때까지
		{
			if ((i + 1) % 2 == 1) //pw[0, 2, 4, 6, 8]
				pw_real[i] = id[id_count++];

			else //pw[1, 3, 5, 7, 9]
				pw_real[i] = domain[domain_count++];
		}
	}

	else if (len_id > len_domain) //id가 domain보다 긴 경우
	{
		for (i = 0; i < 10 && domain[domain_count] != '\0'; i++) //10개를 채우거나 domain의 문자들을 다 사용할때까지
		{
			if ((i + 1) % 2 == 1) //pw[0, 2, 4, 6, 8]
				pw_real[i] = id[id_count++];

			else //pw[1, 3, 5, 7, 9]
				pw_real[i] = domain[domain_count++];
		}

		if (domain[domain_count] == '\0') //10글자를 채우지 못했는데 도메인의 글자가 부족한 경우
		{
			for (; i < 10 && id[id_count] != '\0'; i++) //아이디로만 채움
				pw_real[i] = id[id_count++];
		}
	}

	else //domain이 id보다 긴 경우
	{
		for (i = 0; i < 10 && id[id_count] != '\0'; i++) //10개를 채우거나 id의 문자들을 다 사용할때까지
		{
			if ((i + 1) % 2 == 1) //pw[0, 2, 4, 6, 8]
				pw_real[i] = id[id_count++];

			else //pw[1, 3, 5, 7, 9]
				pw_real[i] = domain[domain_count++];
		}

		if (id[id_count] == '\0') //10글자를 채우지 못했는데 아이디의 글자가 부족한 경우
		{
			for (; i < 10 && domain[domain_count] != '\0'; i++) //도메인으로만 채움
				pw_real[i] = domain[domain_count++];
		}
	}
}

int validate_email(char email[], char pw[])
{
	int i, j, pw_correction;
	int period = 0;
	int start_domain = 0, allow_domain = 0;
	char domain[21]; //도메인최대길이 20 + NULL문자 이므로 배열크기 21
	FILE* file_domain;

	printf("\n--------------CHECKING--------------\n");
	//아이디 부분 체크
	for (i = 0; email[i] != '\0'; i++)
		if (email[i] == '@') break; //어디까지가 아이디인지 확인

	if (email[i] == '\0' || i == 0)
	{
		result(1); //@앞에 아무것도 없는 경우, 아이디 없이 "@(도메인)"형태로 입력된 경우 이메일형식에러
		return 0; //도메인에러 및 도메인허용은 확인할 필요 없음
	}

	if (email[i] == '@' && email[i + 1] == '\0') //@은 있지만 그뒤에 아무것도 없는 경우
	{
		result(1);
		return 0; //도메인에러 및 도메인 허용은 확인할 필요 없음
	}

	else //@이 있는 경우에는 도메인 체크
	{
		start_domain = ++i; //@바로뒤부터 도메인 시작임
		if (email[start_domain] == '.')
		{
			result(2); //@다음 '.'이 나오는 경우 도메인에러
			return 0; //에러확정이라서 도메인허용 검사할 필요 없음
		}

		else //@다음 '.'이 아닌경우
		{
			for (; email[i] != '\0'; i++)
			{
				if (email[i] == '.') period++; //점개수 체크

				if (email[i] == '.' && email[i] == email[i - 1])
				{
					result(2); //연속으로 '.'이 나오는 경우
					return 0; //에러 확정이라서 도메인허용 검사할 필요 없음
				}

				if (email[i] == '.' && email[i + 1] == '\0')
				{
					result(2); //도메인 마지막에 '.'이 나오는 경우 에러
					return 0; //에러 확정이라서 도메인허용 검사할 필요 없음
				}
			}

			if (period == 0)
			{
				result(2); //점이 없으면 도메인, 점이 있을때 생기는 에러는 전부 위에서 처리함
				return 0; //에러확정이라서 도메인허용 검사할 필요 없음
			}
		}
	}

	//여기서부터 허용되는 도메인인지 체크
	file_domain = fopen("allowed_domain.txt", "r"); //파일열기
	fscanf(file_domain, "%d", &j); //도메인 개수 저장
	for (i = 0; i < j; i++)
	{
		while (fscanf(file_domain, "%s", domain) != EOF)
			allow_domain += strcmp(domain, &email[start_domain]);
	} //최종적으로 허용되는 도메인이면 1, 허용안되는 도메인이면 0을 값으로 가지는 allow_domain

	fclose(file_domain);

	if (allow_domain == 0)
	{
		result(3); //도메인허용 안됨
		return 0; //비밀번호 검사할 필요 없음
	}

	else
	{
		pw_correction = pw_check(email, pw, start_domain); //이메일 검사를 모두 통과했으므로 비밀번호 검사 시작

		if (pw_correction == 1) return 1; //비밀번호가 정확하면

		else return 0; //비밀번호가 정확하지 않으면
	}
}

void scan(char email[], char pw[]) //이메일, 비밀번호 입력받음
{
	printf("email : ");
	scanf("%s", email);

	printf("password : ");
	scanf("%s", pw);
}


int pw_check(char email[], char pw[], int start_domain) //입력받은 비밀번호와 실제 비밀번호를 비교함
{
	char pw_real[11] = "0000000000"; //미리 10글자에 모두 0을넣어두면 아이디, 도메인이이 부족하더라도 규칙에 맞게 미리 0이 들어가있음
	char id[27], domain[29]; //id최대 길이 : 도메인이 3글자일때이므로 26 | domain최대 길이 : 아이디가 1글자일때이므로 28 (pw_check함수에 도달했다는 것은 이메일 형식이 온전하다는 것이므로)

	strcpy(id, email); //아이디 배열 만듦
	strcpy(domain, &email[start_domain]); //도메인 배열 만듦

	make_password(id, domain, pw_real); //규칙에 따라 비밀번호 만듦

	if (strcmp(pw, pw_real) == 1) //비밀번호가 정확하면
	{
		result(4);
		return 1;
	}

	else //비밀번호가 정확하지 않으면
	{
		result(5);
		return 0;
	}
}

void result(int res) //결과출력
{
	switch (res) {
	case 1: printf("ERROR! invalid email format!\n"); break;
	case 2: printf("ERROR! invalid domain format!\n"); break;
	case 3: printf("ERROR! This domain is not allowed!\n"); break;
	case 4: printf("SUCCESS! now you can access HEA system!\n\n"); break; //그림3에서 2줄 띄어져있음
	case 5: printf("ERROR! invalid password!\n"); break;
	}
}

int strcmp(const char s1[], const char s2[]) //문자열 비교
{
	int i;

	for (i = 0; s1[i] != '\0' && s2[i] != '\0'; i++) //문자열 둘중 하나가 끝날때까지 반복
		if (s1[i] != s2[i]) return 0; //성립안하면 다른 도메인이므로 0을 리턴

	if ((s1[i] == '\0' && s2[i] != '\0') || (s1[i] != '\0' && s2[i] == '\0')) //도메인 길이가 다르면 다른도메인이므로 0을 리턴
		return 0;
	
	return 1; //같은 도메인이면 1을 리턴
}

int strlen(const char str[]) //문자열 길이 반환
{
	int length;

	for (length = 0; str[length] != '\0'; length++) //for문을 나가면 i값은 id 또는 domain의 길이가 됨
		;

	return length; // 길이 반환
}

void strcpy(char copyed[], char original[]) //문자열 복사
{
	int i;
	
	for (i = 0; original[i] != '\0' && original[i] != '@'; i++) //이메일 전체배열에서 @앞인 아이디부분을 복사하거나 @뒤인 도메인부분을 복사할때 사용할 것이므로
		copyed[i] = original[i];

	if (original[i] == '@') //아이디복사
		copyed[i] = '\0';

	else if (original[i] == '\0') //도메인복사
		copyed[i] = '\0';
}