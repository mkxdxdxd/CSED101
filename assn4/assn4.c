#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//USER ����ü ����
typedef struct User {
	char name[21];
	char email[41];
	struct User* next;
} USER;

//PARTICIPANT ����ü ����
typedef struct Participant {
	USER* user;
	bool camera;
	bool microphone;
	struct Participant* next;
} PARTICIPANT;

//CONFERENCCE ����ü ����
typedef struct Conference {
	char roomName[21];
	int numParticipant;
	int maxParticipant;
	PARTICIPANT* participants;
} CONFERENCE;

void swap(USER* plist, int num);//���ĺ� ������ �� ���Ǵ� �Լ�
void user_list(USER* plist);//user list ��� �����ϴ� �Լ�
void user_add(USER* plist, int* num);//user add ��� �����ϴ� �Լ�
void user_delete(USER* plist, int* num);//user delete ��� �����ϴ� �Լ�
void conf_info(CONFERENCE* conf);//conf info ��� �����ϴ� �Լ�
void conf_join(USER* plist, CONFERENCE* conf);// conf join ��� �����ϴ� �Լ�
void conf_hangup(CONFERENCE* conf);// hang up ��� �����ϴ� �Լ�
void conf_toggle_camera(CONFERENCE* conf);//cam on off �ϴ� �Լ�
void conf_toggle_mic(CONFERENCE* conf);//mic on off �ϴ� �Լ�

int main()
{
	USER *NewUser;//User �߰��Ҷ� ���̴� ������
	USER *temp;//User �߰��� �� ���̴� ������
	CONFERENCE cinfo;//ȸ�� ���� �߰��Ҷ� ���̴� ����ü
	char filename[31];//�о�� ���ϸ�
	char str[61];//���Ͽ��� ���پ� �о�ö� ���̴� �迭
	char* ptr;//�ڸ� ��ū�� ����ų ����
	int num = 0;//��� ���� ������ ������ ������ �о���� ����
	char command[26];//��ɹ� �о���� �迭
	
	//ȸ�� ���� �ʱ�ȭ
	cinfo.numParticipant = 0;
	cinfo.maxParticipant = 0;
	cinfo.participants = NULL;

	//���ϸ� ��ĵ�ؿ���
	while (1) {
		printf("���� ����Ʈ ���� �̸��� �Է����ּ���. >> ");
		fseek(stdin, 0, SEEK_END);
		scanf("%s", filename);

		FILE* fp = fopen(filename, "r");
		if (fp == NULL)
		{
			printf("��ȿ���� ���� �Է��Դϴ�. ");
			continue;
		}
		else
			break;
		fclose(fp);
	}

	//���� ����
	FILE* fp = fopen(filename, "r");

	//User node �Ҵ�
	NewUser = (USER*)malloc(sizeof(USER));
	//�ʱ�ȭ
	NewUser->next = NULL;
	strcpy(NewUser->name, "\0");
	strcpy(NewUser->email, "\0");

	//�о�� ���� strtok�� �߶� USER�� �ֱ�
	USER* pPre;
	while (fgets(str, 61, fp) != NULL)
	{
		temp = (USER*)malloc(sizeof(USER));
		temp->next = NULL;
		if (NewUser->next == NULL)
		{
			//user ���� ����Ʈ�� �������
			ptr = strtok(str, "\t");
			strcpy(temp->name, ptr);
			ptr = strtok(NULL, "\n");
			strcpy(temp->email, ptr);
			NewUser->next = temp;
		}
		else
		{
			//���� ����Ʈ�� �ϳ� �̻��� ����Ǿ� ������ 
			ptr = strtok(str, "\t");
			strcpy(temp->name, ptr);
			ptr = strtok(NULL, "\n");
			strcpy(temp->email, ptr);

			pPre = NewUser->next;
			while (1)
			{
				if (pPre->next == NULL)
				{
					pPre->next = temp;
					break;
				}
				pPre = pPre->next;
			}
		}
		num++;
	}
	//������ ���� ����Ʈ ���ĺ� ������� �ٲٱ� 
	swap(NewUser,num);

	//ȸ�ǽ� ���� �Է�
	printf("\n[INFO] user.txt���� �� %d���� ������ �ҷ��Խ��ϴ�.\n\n",num);
	printf("ȸ�ǽ� �̸��� �Է����ּ���. >> ");
	fseek(stdin, 0, SEEK_END);
	scanf("%s", cinfo.roomName);
	printf("�ִ� ����� ���� �Է����ּ���. >> ");
	fseek(stdin, 0, SEEK_END);
	scanf("%d", &cinfo.maxParticipant);		
	printf("[INFO] %s ȸ�ǽ��� �����Ǿ����ϴ�!",cinfo.roomName);

	//��ɹ� �Է�
	while (1)
	{
		printf("\n\n��ɾ �Է����ּ���. >> ");
		fseek(stdin, 0, SEEK_END);
		fgets(command, 26, stdin);

		if (strcmp(command, "user list\n") == 0) user_list(NewUser);
		else if (strcmp(command, "user add\n") == 0) user_add(NewUser, &num);
		else if (strcmp(command, "user delete\n") == 0) user_delete(NewUser, &num);
		else if (strcmp(command, "conf info\n") == 0) conf_info(&cinfo);
		else if (strcmp(command, "conf join\n") == 0) conf_join(NewUser,&cinfo);
		else if (strcmp(command, "conf hangup\n") == 0) conf_hangup(&cinfo);
		else if (strcmp(command, "conf toggle camera\n") == 0) conf_toggle_camera(&cinfo);
		else if (strcmp(command, "conf toggle mic\n") == 0) conf_toggle_mic(&cinfo);
		else if (strcmp(command, "quit\n") == 0) break;
		else printf("��ȿ���� ���� �Է��Դϴ�. ");
	}

	//�Ҵ��� ���Ḯ��Ʈ �� �Ҵ� ���� �ؾ��Ѵ�.
	USER* utarget = NewUser;
	USER* utemp = utarget;
	while (utarget!=NULL)
	{
		utemp = utemp->next;
		free(utarget);
		utarget = utemp;
	}

	PARTICIPANT* ptarget= cinfo.participants;
	PARTICIPANT* ptemp = ptarget;
	while (ptarget != NULL)
	{
		ptemp = ptemp->next;
		free(ptarget);
		ptarget = ptemp;
	}
}

//user_list �Լ�
void user_list(USER* plist)
{
	USER* temp;//user ���� ����Ʈ�� �ϴϾ� ����Ű�鼭 ����Ѵ�.
	int i = 0;
	printf("\n===============================================================\n");
	printf("��ȣ             �̸�                                    �̸���\n");
	printf("---------------------------------------------------------------\n");

	//temp�� ����Ű�� ���� ����Ʈ�� NULL�� �ƴҶ����� �ݺ��� ������. 
	for (temp = plist->next; temp != NULL; temp = temp->next)
	{
		i++;
		printf("%d %20s %40s\n", i, temp->name, temp->email);
	}
	printf("===============================================================");
}

void user_add(USER* plist, int* num)
{
	USER* temp; //�߰��ϰ� ���� ������ ������ ������ temp ����

	//�Ҵ�� �ʱ�ȭ
	temp = (USER*)malloc(sizeof(USER));
	temp->next = NULL;
	strcpy(temp->email, "\0");
	strcpy(temp->name, "\0");

	//������ node�� ã�Ƴ� ����
	USER* pPre;

	//�߰��� ���� �̸� ��ĵ
	printf("�߰��� ������ �̸��� �Է����ּ���. >> ");
	fseek(stdin, 0, SEEK_END);
	scanf("%s", temp->name);
	
	//���� �Է��� ������ �̹� ���� ����Ʈ�� ���� ���
	for (pPre = plist->next; pPre != NULL; pPre = pPre->next)
	{
		if (strcmp(temp->name, pPre->name) == 0)
		{
			printf("[INFO] %s, %s ������ �̹� ���� ����Ʈ�� �ֽ��ϴ�.", pPre->name, pPre->email);
			return;
		}
	}

	//�̸��� ��ĵ
	printf("�߰��� ������ �̸����� �Է����ּ���. >> ");
	fseek(stdin, 0, SEEK_END);
	scanf("%s", temp->email);
	printf("[INFO] %s, %s ������ �߰��Ǿ����ϴ�.", temp->name, temp->email);
	
	//��ĵ�� ����(�̸���, �̸�)�� ���� ���Ḯ��Ʈ�� �����Ѵ�. 
	pPre = plist->next;
	while (1)
	{
		if (pPre->next == NULL)
		{
			pPre->next = temp;
			*num = *num + 1;
			break;
		}
		pPre = pPre->next;
	}

	//���� �� ���ĺ� ������ �ٽ� ����
	swap(plist, *num);
}

//user delete ��� ó���ϴ� �Լ�
void user_delete(USER* plist, int* num)
{
	USER* pos;
	USER* temp;
	char name[21];//������ ������ �̸��� ��ĵ�� �Լ�
	char yn;//���� �����Ͻðڽ��ϱ�� y/n ����

	printf("������ ������ �̸��� �Է����ּ���. >> ");
	fseek(stdin, 0, SEEK_END);
	scanf("%s", name);

	//������ ������ ���� ���� ����Ʈ���� ã�´�
	for (pos = plist->next; pos != NULL; pos = pos->next)
	{
		//���� ��ĵ�� �̸��� ��������Ʈ�� ���ٸ�
		if (strcmp(name, pos->name) == 0)
		{
			printf("%s, %s ������ �����Ͻðڽ��ϱ�? (y/n) >> ", pos->name, pos->email);
			fseek(stdin, 0, SEEK_END);
			scanf("%c", &yn);
			if (yn == 'y')
			{
				printf("[INFO] %s, %s ������ �����Ͽ����ϴ�.", pos->name, pos->email);
				//���� �� �پ���
				*num = *num - 1;
				//���� �Է��� ���� ù��° node�� ������ִٸ�
				if (plist->next == pos) 
				{
					plist->next = pos->next;
					free(pos);
				}
				//�Է��� ���� ù��° node�� �ƴ϶��
				else
				{
					//temp�� ���� ��ġ�ϴ� ��带 ã�´�.
					for (temp = plist->next; temp->next != NULL; temp = temp->next)
					{
						//��ġ�ϴ� ��带 ã���� break
						if (temp->next == pos)
						{
							break;
						}
					}
					//��ġ�ϴ� ��带 �����Ѵ�.
					if (temp->next == pos)
					{
						temp->next = pos->next;
						free(pos);
					}
				}
				return;
			}
			else if (yn == 'n')
			{
				return;
			}
		}
	}
	 printf("[INFO] �ش� ������ �������� �ʽ��ϴ�!");
}

void swap(USER* plist, int num)
{
	USER* temp;//������ �ӽ÷� ������ ��� 
	//�Ҵ�� �ʱ�ȭ
	temp = (USER*)malloc(sizeof(USER));
	strcpy(temp->name, "\0");
	strcpy(temp->email, "\0");
	temp->next = NULL;

	//swap �Լ� ����
	USER* a;
	USER* b;

	//���� ���� ����
	for (int i = 0; i < num; i++)
	{
		a = plist->next;
		b = plist->next->next;

		for (int j = 0; j < num - i - 1; j++)
		{
			if (strcmp(a->name, b->name) == 1)
			{
				strcpy(temp->name, b->name);
				strcpy(temp->email, b->email);
				strcpy(b->name, a->name);
				strcpy(b->email, a->email);
				strcpy(a->name, temp->name);
				strcpy(a->email, temp->email);
			}
			a = a->next;
			b = b->next;
		}
	}
	//�Ҵ� ����
	free(temp);
}

//ȸ�� ����
void conf_info(CONFERENCE *conf)
{
	PARTICIPANT* temp; //participant ���� ����Ʈ�� temp�� �ű�鼭 ����Ѵ�
	printf("ȸ�ǽ� �̸�: %s\n",conf->roomName);
	printf("�ִ� ����ڼ�: %d\n",conf->maxParticipant);
	printf("������ ���:\n");
	printf("================================================================================\n");
	printf("                �̸�                                   �̸���    ī�޶�   ����ũ\n");
	printf("--------------------------------------------------------------------------------\n");
	//participant ���� ����Ʈ�� temp�� �ű�鼭 ����Ѵ�. 
	for (temp = conf->participants; temp != NULL; temp = temp->next)
	{
		printf("%20s %40s", temp->user->name, temp->user->email);
		if (temp->camera == true) printf("%10s", "ON");
		else printf("%10s", "OFF");
		if (temp->microphone == true) printf("%10s", "ON\n");
		else printf("%10s", "OFF\n");
	}
	printf("================================================================================\n");
}

//ȸ�ǿ� ���� �ִ� �Լ�
void conf_join(USER *plist,CONFERENCE *conf)
{	
	//participant ���� �̹� max num�� �ٴ����� ���
	if (conf->maxParticipant == conf->numParticipant)
	{
		printf("[INFO] ȸ�ǿ� �ִ� �ο� %d ���� �����ϰ� �ֽ��ϴ�.",conf->maxParticipant);
		return;
	}

	char name[21];//�̸� 20����
	char yn;//ī�޶� ����ũ ���� �Ǵ�
	PARTICIPANT* NewParticipants;//���ο� ���� participant list �� �����ϴ� ����
	PARTICIPANT* ptemp;//
	USER* utemp;//�Է��� ������ participant list �� �ִ��� �Ǵ��ϴ� ������

	//���ο� ������ node �Ҵ�� �ʱ�ȭ
	NewParticipants = (PARTICIPANT*)malloc(sizeof(PARTICIPANT));
	NewParticipants->next = NULL;
	NewParticipants->user = NULL;

	//�߰��� ������ �̸� �Է�
	printf("�߰��� ������ �̸��� �Է����ּ���. >> ");
	fseek(stdin, 0, SEEK_END);
	scanf("%s", name);

	//���� ������ ���ٸ� ���ٴ� �޽��� ����ϱ�
	utemp = plist->next;
	while (1)
	{
		if (strcmp(name, utemp->name) == 0)
		{
			break;
		}
		utemp = utemp->next;
		if (utemp == NULL)
		{
			printf("[INFO] �ش� ������ �������� �ʽ��ϴ�!");
			return;
		}
	}

	//�Է��� �̸��� �̹� participant list�� �ִٸ� �̹� �����ߴٴ� �޽��� ����ϱ�
	for (ptemp = conf->participants; ptemp != NULL; ptemp = ptemp->next)
	{
		if (strcmp(name, ptemp->user->name) == 0)
		{
			printf("[INFO] %s�̰� �̹� ȸ�ǿ� �����߽��ϴ�!", name);
			return;
		}
	}

	//ī�޶� ����ũ on off ���� Ȯ�� �� NewParticiapnt node�� ���� �߰��ϱ�
	printf("ī�޶� �� ���·� �����Ͻðڽ��ϱ�? (y/n) >> ");
	fseek(stdin, 0, SEEK_END);
	scanf("%c", &yn);
	if (yn == 'y')
	{
		NewParticipants->camera = true;
	}
	else if (yn == 'n')
	{
		NewParticipants->camera = false;
	}
	printf("����ũ�� �� ���·� �����Ͻðڽ��ϱ�? (y/n) >> ");
	fseek(stdin, 0, SEEK_END);
	scanf("%c", &yn);
	if (yn == 'y')
	{
		NewParticipants->microphone = true;
	}
	else if (yn == 'n')
	{
		NewParticipants->microphone = false;
	}

	//���ο� �����ڶ�� ����Ʈ�� �߰�
	//ù��° �����ڶ��
	if (conf->participants == NULL)
	{
		conf->participants = NewParticipants;
		conf->numParticipant++;
	}
	//�ι�°�� ���� ����Ʈ�� �����ϴ� ���̶�� 
	else
	{
		ptemp = conf->participants;
		while (1)
		{
			if (ptemp->next == NULL)
			{
				ptemp->next = NewParticipants;
				break;
			}
			ptemp = ptemp->next;
		}
		conf->numParticipant++;
	}


	//user list�� �Էµ� ��� �ִ��� ã���� participant node�� user�� ����Ű��
	for (utemp = plist->next; utemp != NULL; utemp = utemp->next)
	{
		//user list���� ���� ��� ã�Ƽ� ���� ������ participants�� �����ϱ�
		if (strcmp(name, utemp->name) == 0)
		{
			//���� ���� �����ϰ� ī�޶� �Է¹��� ��. 
			NewParticipants->user = utemp;
			printf("[INFO] %s ��(��) ȸ�ǿ� �����߽��ϴ�!",utemp->name);
			return;
		}
	}
	return;
}

// participant list ���� ���� �����ϱ�
void conf_hangup(CONFERENCE* conf)
{
	PARTICIPANT* pos;
	PARTICIPANT* temp;
	char name[21];
	char yn;

	printf("������ ������ �̸��� �Է����ּ���. >> ");
	fseek(stdin, 0, SEEK_END);
	scanf("%s", name);
	for (pos = conf->participants; pos != NULL; pos = pos->next)
	{
		if (strcmp(name, pos->user->name) == 0)
		{
			printf("������ ȸ�ǿ��� �������ðڽ��ϱ�? (y/n) >> ");
			fseek(stdin, 0, SEEK_END);
			scanf("%c", &yn);
			if (yn == 'y')
			{
				printf("[INFO] %s ��(��) ȸ�ǿ��� �������ϴ�!", name);
				//ù��° ������ ��� �����ϴ� ���
				if (conf->participants == pos)
				{
					conf->participants = pos->next;
					free(pos);
				}
				//ù��°�� �ƴ� �� ���� ������ ��� �����ϴ� ���
				else
				{
					//temp�� ���� ��ġ�ϴ� ��带 ã�´�.
					for (temp = conf->participants; temp->next != NULL; temp = temp->next)
					{
						//��ġ�ϸ� break
						if (temp->next == pos)
						{
							break;
						}
					}
					//node �����ϰ� free �ϱ�
					if (temp->next == pos)
					{
						temp->next = pos->next;
						free(pos);
					}
				}
				conf->numParticipant--;
				return;
			}
			else if (yn == 'n')
			{
				printf("\n\n");
				return;
			}
		}
	}
	printf("[INFO] �ش� ������ ȸ�ǿ� �������� �ʽ��ϴ�!");
}

//ī�޶� on off ���� �ٲٴ� �Լ�
void conf_toggle_camera(CONFERENCE* conf)
{
	char name[21];// ���� �ٲٰ� ���� ���� �̸� ��ĵ
	PARTICIPANT* temp;//conference node�� ����Ű�鼭 ���� �ٲٱ�
	printf("ī�޶� ���¸� ������ ������ �̸��� �Է����ּ���. >> ");
	scanf("%s", name);
	for (temp = conf->participants; temp != NULL; temp = temp->next)
	{
		//���� �Է��� �̸��� user list�̸��� ��ġ�Ѵٸ�
		if (strcmp(name, temp->user->name) == 0)
		{
			//����ũ�� �����ִ� ���
			if (temp->camera == true)
			{
				temp->camera = false;
				printf("[INFO] %s�� ī�޶� ON->OFF �Ǿ����ϴ�!", name);
			}
			//����ũ�� �����ִ� ���
			else if (temp->camera == false)
			{
				temp->camera = true;
				printf("[INFO] %s�� ī�޶� OFF->ON �Ǿ����ϴ�!", name);
			}
			return;
		}
	}
	printf("[INFO] �ش� ������ ȸ�ǿ� �������� �ʽ��ϴ�!");
}

//����ũ ���� on off �ϴ� �Լ�
void conf_toggle_mic(CONFERENCE* conf)
{
	char name[21];//���¸� �ٲٰ� ���� ������ �̸� ��ĵ
	PARTICIPANT* temp; // conference node�� ����Ű�鼭 ���� �ٲٱ�
	printf("����ũ ���¸� ������ ������ �̸��� �Է����ּ���. >> ");
	scanf("%s", name);
	for (temp = conf->participants; temp != NULL; temp = temp->next)
	{
		//���� �Է��� �̸��� user list�̸��� ��ġ�Ѵٸ�
		if (strcmp(name, temp->user->name) == 0)
		{
			//����ũ �����ִ� ���
			if (temp->microphone == true)
			{
				temp->microphone = false;
				printf("[INFO] %s�� ����ũ�� ON->OFF �Ǿ����ϴ�!", name);
			}
			//����ũ �����ִ� ���
			else if (temp->microphone == false)
			{
				temp->microphone = true;
				printf("[INFO] %s�� ����ũ�� OFF->ON �Ǿ����ϴ�!", name);
			}
			return;
		}
	}
	printf("[INFO] �ش� ������ ȸ�ǿ� �������� �ʽ��ϴ�!");
}