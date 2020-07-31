#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define EMAIL_MAX 30 //�̸��� �ִ����
#define PW_MAX 30 //��й�ȣ �ִ����

void make_password(char id[], char domain[], char pw_real[]); //�̸��Ϸκ��� �н����� �����ϴ� �Լ�
int validate_email(char email[], char pw[]); //�̸���������ġ, ��뵵�������� üũ�ϴ� �Լ�

void scan(char email[], char pw[]); //�̸���, ��й�ȣ �Է¹���
int pw_check(char email[], char pw[], int start_domain); //�Է¹��� ��й�ȣ�� ���� ��й�ȣ�� ����

void result(int res); //������

int strcmp(const char s1[], const char s2[]); //���ڿ� ��
int strlen(const char str[]); //���ڿ� ���� ��ȯ
void strcpy(char copyed[], char original[]); //���ڿ� ����

int main()
{
	int res;
	char re;
	char email[EMAIL_MAX+1]; //�̸����� 30�ڰ� �ԷµǴ°�� '\0'�� �� ������ �־�� �ϹǷ� ũ��� �ִ���� +1�� �����Ѵ�
	char pw[PW_MAX+1]; //��й�ȣ�� ��������

	while (1)
	{
		printf("---LOGIN TO ACCESS HEA SYSTEM---\n\n");
		scan(email, pw); //�Է¹���
		res = validate_email(email, pw); //������������ Ȯ�θ���

		if (res != 1) //��������������
		{
			printf("\nDo you want try again? (y/n) ");
			scanf(" %c", &re); //���Է��ǻ� ���, " %c"�� ����Ͽ� ���ۿ� �����ִ� ���๮�� ����

			if (re == 'y')
				printf("\n"); //�ٹٲ��ְ� ó������ �ٽ� ����

			else if (re == 'n') //n�� �ԷµǸ� ����
				break; //���α׷��� �����ϱ� ���� �ݺ��� Ż��
		}

		else break; //��й�ȣ�� ��Ȯ�ϸ� ���α׷� �����ؾ��ϹǷ� �ݺ��� Ż��
	}
	return 0;
}

void make_password(char id[], char domain[], char pw_real[]) //�̸��Ϸκ��� real password ����
{
	int i, len_id, len_domain, id_count = 0, domain_count = 0;

	len_id = strlen(id); //id�� ���̰� ����ȴ�
	len_domain = strlen(domain); //domain�� ���̰� ����ȴ�

	if (len_id == len_domain) //id, domain ���� ���� ���
	{
		for (i = 0; i < 10 && domain[domain_count] != '\0'; i++) //10���� ä��ų� id, domain�� ���ڵ��� �� ����Ҷ�����
		{
			if ((i + 1) % 2 == 1) //pw[0, 2, 4, 6, 8]
				pw_real[i] = id[id_count++];

			else //pw[1, 3, 5, 7, 9]
				pw_real[i] = domain[domain_count++];
		}
	}

	else if (len_id > len_domain) //id�� domain���� �� ���
	{
		for (i = 0; i < 10 && domain[domain_count] != '\0'; i++) //10���� ä��ų� domain�� ���ڵ��� �� ����Ҷ�����
		{
			if ((i + 1) % 2 == 1) //pw[0, 2, 4, 6, 8]
				pw_real[i] = id[id_count++];

			else //pw[1, 3, 5, 7, 9]
				pw_real[i] = domain[domain_count++];
		}

		if (domain[domain_count] == '\0') //10���ڸ� ä���� ���ߴµ� �������� ���ڰ� ������ ���
		{
			for (; i < 10 && id[id_count] != '\0'; i++) //���̵�θ� ä��
				pw_real[i] = id[id_count++];
		}
	}

	else //domain�� id���� �� ���
	{
		for (i = 0; i < 10 && id[id_count] != '\0'; i++) //10���� ä��ų� id�� ���ڵ��� �� ����Ҷ�����
		{
			if ((i + 1) % 2 == 1) //pw[0, 2, 4, 6, 8]
				pw_real[i] = id[id_count++];

			else //pw[1, 3, 5, 7, 9]
				pw_real[i] = domain[domain_count++];
		}

		if (id[id_count] == '\0') //10���ڸ� ä���� ���ߴµ� ���̵��� ���ڰ� ������ ���
		{
			for (; i < 10 && domain[domain_count] != '\0'; i++) //���������θ� ä��
				pw_real[i] = domain[domain_count++];
		}
	}
}

int validate_email(char email[], char pw[])
{
	int i, j, pw_correction;
	int period = 0;
	int start_domain = 0, allow_domain = 0;
	char domain[21]; //�������ִ���� 20 + NULL���� �̹Ƿ� �迭ũ�� 21
	FILE* file_domain;

	printf("\n--------------CHECKING--------------\n");
	//���̵� �κ� üũ
	for (i = 0; email[i] != '\0'; i++)
		if (email[i] == '@') break; //�������� ���̵����� Ȯ��

	if (email[i] == '\0' || i == 0)
	{
		result(1); //@�տ� �ƹ��͵� ���� ���, ���̵� ���� "@(������)"���·� �Էµ� ��� �̸������Ŀ���
		return 0; //�����ο��� �� ����������� Ȯ���� �ʿ� ����
	}

	if (email[i] == '@' && email[i + 1] == '\0') //@�� ������ �׵ڿ� �ƹ��͵� ���� ���
	{
		result(1);
		return 0; //�����ο��� �� ������ ����� Ȯ���� �ʿ� ����
	}

	else //@�� �ִ� ��쿡�� ������ üũ
	{
		start_domain = ++i; //@�ٷεں��� ������ ������
		if (email[start_domain] == '.')
		{
			result(2); //@���� '.'�� ������ ��� �����ο���
			return 0; //����Ȯ���̶� ��������� �˻��� �ʿ� ����
		}

		else //@���� '.'�� �ƴѰ��
		{
			for (; email[i] != '\0'; i++)
			{
				if (email[i] == '.') period++; //������ üũ

				if (email[i] == '.' && email[i] == email[i - 1])
				{
					result(2); //�������� '.'�� ������ ���
					return 0; //���� Ȯ���̶� ��������� �˻��� �ʿ� ����
				}

				if (email[i] == '.' && email[i + 1] == '\0')
				{
					result(2); //������ �������� '.'�� ������ ��� ����
					return 0; //���� Ȯ���̶� ��������� �˻��� �ʿ� ����
				}
			}

			if (period == 0)
			{
				result(2); //���� ������ ������, ���� ������ ����� ������ ���� ������ ó����
				return 0; //����Ȯ���̶� ��������� �˻��� �ʿ� ����
			}
		}
	}

	//���⼭���� ���Ǵ� ���������� üũ
	file_domain = fopen("allowed_domain.txt", "r"); //���Ͽ���
	fscanf(file_domain, "%d", &j); //������ ���� ����
	for (i = 0; i < j; i++)
	{
		while (fscanf(file_domain, "%s", domain) != EOF)
			allow_domain += strcmp(domain, &email[start_domain]);
	} //���������� ���Ǵ� �������̸� 1, ���ȵǴ� �������̸� 0�� ������ ������ allow_domain

	fclose(file_domain);

	if (allow_domain == 0)
	{
		result(3); //��������� �ȵ�
		return 0; //��й�ȣ �˻��� �ʿ� ����
	}

	else
	{
		pw_correction = pw_check(email, pw, start_domain); //�̸��� �˻縦 ��� ��������Ƿ� ��й�ȣ �˻� ����

		if (pw_correction == 1) return 1; //��й�ȣ�� ��Ȯ�ϸ�

		else return 0; //��й�ȣ�� ��Ȯ���� ������
	}
}

void scan(char email[], char pw[]) //�̸���, ��й�ȣ �Է¹���
{
	printf("email : ");
	scanf("%s", email);

	printf("password : ");
	scanf("%s", pw);
}


int pw_check(char email[], char pw[], int start_domain) //�Է¹��� ��й�ȣ�� ���� ��й�ȣ�� ����
{
	char pw_real[11] = "0000000000"; //�̸� 10���ڿ� ��� 0���־�θ� ���̵�, ���������� �����ϴ��� ��Ģ�� �°� �̸� 0�� ������
	char id[27], domain[29]; //id�ִ� ���� : �������� 3�����϶��̹Ƿ� 26 | domain�ִ� ���� : ���̵� 1�����϶��̹Ƿ� 28 (pw_check�Լ��� �����ߴٴ� ���� �̸��� ������ �����ϴٴ� ���̹Ƿ�)

	strcpy(id, email); //���̵� �迭 ����
	strcpy(domain, &email[start_domain]); //������ �迭 ����

	make_password(id, domain, pw_real); //��Ģ�� ���� ��й�ȣ ����

	if (strcmp(pw, pw_real) == 1) //��й�ȣ�� ��Ȯ�ϸ�
	{
		result(4);
		return 1;
	}

	else //��й�ȣ�� ��Ȯ���� ������
	{
		result(5);
		return 0;
	}
}

void result(int res) //������
{
	switch (res) {
	case 1: printf("ERROR! invalid email format!\n"); break;
	case 2: printf("ERROR! invalid domain format!\n"); break;
	case 3: printf("ERROR! This domain is not allowed!\n"); break;
	case 4: printf("SUCCESS! now you can access HEA system!\n\n"); break; //�׸�3���� 2�� ���������
	case 5: printf("ERROR! invalid password!\n"); break;
	}
}

int strcmp(const char s1[], const char s2[]) //���ڿ� ��
{
	int i;

	for (i = 0; s1[i] != '\0' && s2[i] != '\0'; i++) //���ڿ� ���� �ϳ��� ���������� �ݺ�
		if (s1[i] != s2[i]) return 0; //�������ϸ� �ٸ� �������̹Ƿ� 0�� ����

	if ((s1[i] == '\0' && s2[i] != '\0') || (s1[i] != '\0' && s2[i] == '\0')) //������ ���̰� �ٸ��� �ٸ��������̹Ƿ� 0�� ����
		return 0;
	
	return 1; //���� �������̸� 1�� ����
}

int strlen(const char str[]) //���ڿ� ���� ��ȯ
{
	int length;

	for (length = 0; str[length] != '\0'; length++) //for���� ������ i���� id �Ǵ� domain�� ���̰� ��
		;

	return length; // ���� ��ȯ
}

void strcpy(char copyed[], char original[]) //���ڿ� ����
{
	int i;
	
	for (i = 0; original[i] != '\0' && original[i] != '@'; i++) //�̸��� ��ü�迭���� @���� ���̵�κ��� �����ϰų� @���� �����κκ��� �����Ҷ� ����� ���̹Ƿ�
		copyed[i] = original[i];

	if (original[i] == '@') //���̵𺹻�
		copyed[i] = '\0';

	else if (original[i] == '\0') //�����κ���
		copyed[i] = '\0';
}