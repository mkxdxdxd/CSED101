#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//USER 구조체 선언
typedef struct User {
	char name[21];
	char email[41];
	struct User* next;
} USER;

//PARTICIPANT 구조체 선언
typedef struct Participant {
	USER* user;
	bool camera;
	bool microphone;
	struct Participant* next;
} PARTICIPANT;

//CONFERENCCE 구조체 선언
typedef struct Conference {
	char roomName[21];
	int numParticipant;
	int maxParticipant;
	PARTICIPANT* participants;
} CONFERENCE;

void swap(USER* plist, int num);//알파벳 정렬할 떄 사용되는 함수
void user_list(USER* plist);//user list 기능 수행하는 함수
void user_add(USER* plist, int* num);//user add 기능 수행하는 함수
void user_delete(USER* plist, int* num);//user delete 기능 수행하는 함수
void conf_info(CONFERENCE* conf);//conf info 기능 수행하는 함수
void conf_join(USER* plist, CONFERENCE* conf);// conf join 기능 수행하는 함수
void conf_hangup(CONFERENCE* conf);// hang up 기능 수행하는 함수
void conf_toggle_camera(CONFERENCE* conf);//cam on off 하는 함수
void conf_toggle_mic(CONFERENCE* conf);//mic on off 하는 함수

int main()
{
	USER *NewUser;//User 추가할때 쓰이는 포인터
	USER *temp;//User 추가할 때 쓰이는 포인터
	CONFERENCE cinfo;//회의 정보 추가할때 쓰이는 구조체
	char filename[31];//읽어올 파일명
	char str[61];//파일에서 한줄씩 읽어올때 쓰이는 배열
	char* ptr;//자른 토큰을 가리킬 변수
	int num = 0;//몇개의 유저 정보를 저장할 것인지 읽어오는 변수
	char command[26];//명령문 읽어오는 배열
	
	//회의 정보 초기화
	cinfo.numParticipant = 0;
	cinfo.maxParticipant = 0;
	cinfo.participants = NULL;

	//파일명 스캔해오기
	while (1) {
		printf("유저 리스트 파일 이름을 입력해주세요. >> ");
		fseek(stdin, 0, SEEK_END);
		scanf("%s", filename);

		FILE* fp = fopen(filename, "r");
		if (fp == NULL)
		{
			printf("유효하지 않은 입력입니다. ");
			continue;
		}
		else
			break;
		fclose(fp);
	}

	//파일 열기
	FILE* fp = fopen(filename, "r");

	//User node 할당
	NewUser = (USER*)malloc(sizeof(USER));
	//초기화
	NewUser->next = NULL;
	strcpy(NewUser->name, "\0");
	strcpy(NewUser->email, "\0");

	//읽어온 정보 strtok로 잘라서 USER에 넣기
	USER* pPre;
	while (fgets(str, 61, fp) != NULL)
	{
		temp = (USER*)malloc(sizeof(USER));
		temp->next = NULL;
		if (NewUser->next == NULL)
		{
			//user 연결 리스트가 비었을때
			ptr = strtok(str, "\t");
			strcpy(temp->name, ptr);
			ptr = strtok(NULL, "\n");
			strcpy(temp->email, ptr);
			NewUser->next = temp;
		}
		else
		{
			//연결 리스트에 하나 이상이 연결되어 있을때 
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
	//저장한 연결 리스트 알파벳 순서대로 바꾸기 
	swap(NewUser,num);

	//회의실 정보 입력
	printf("\n[INFO] user.txt에서 총 %d명의 유저를 불러왔습니다.\n\n",num);
	printf("회의실 이름을 입력해주세요. >> ");
	fseek(stdin, 0, SEEK_END);
	scanf("%s", cinfo.roomName);
	printf("최대 사용자 수를 입력해주세요. >> ");
	fseek(stdin, 0, SEEK_END);
	scanf("%d", &cinfo.maxParticipant);		
	printf("[INFO] %s 회의실이 생성되었습니다!",cinfo.roomName);

	//명령문 입력
	while (1)
	{
		printf("\n\n명령어를 입력해주세요. >> ");
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
		else printf("유효하지 않은 입력입니다. ");
	}

	//할당한 연결리스트 다 할당 해제 해야한다.
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

//user_list 함수
void user_list(USER* plist)
{
	USER* temp;//user 연결 리스트를 하니씩 가르키면서 출력한다.
	int i = 0;
	printf("\n===============================================================\n");
	printf("번호             이름                                    이메일\n");
	printf("---------------------------------------------------------------\n");

	//temp가 가리키는 연결 리스트가 NULL이 아닐때까지 반복문 돌린다. 
	for (temp = plist->next; temp != NULL; temp = temp->next)
	{
		i++;
		printf("%d %20s %40s\n", i, temp->name, temp->email);
	}
	printf("===============================================================");
}

void user_add(USER* plist, int* num)
{
	USER* temp; //추가하고 싶은 유저의 정보를 저장할 temp 변수

	//할당과 초기화
	temp = (USER*)malloc(sizeof(USER));
	temp->next = NULL;
	strcpy(temp->email, "\0");
	strcpy(temp->name, "\0");

	//마지막 node를 찾아낼 변수
	USER* pPre;

	//추가할 유저 이름 스캔
	printf("추가할 유저의 이름을 입력해주세요. >> ");
	fseek(stdin, 0, SEEK_END);
	scanf("%s", temp->name);
	
	//만약 입력한 유저가 이미 연결 리스트에 있을 경우
	for (pPre = plist->next; pPre != NULL; pPre = pPre->next)
	{
		if (strcmp(temp->name, pPre->name) == 0)
		{
			printf("[INFO] %s, %s 유저가 이미 유저 리스트에 있습니다.", pPre->name, pPre->email);
			return;
		}
	}

	//이메일 스캔
	printf("추가할 유저의 이메일을 입력해주세요. >> ");
	fseek(stdin, 0, SEEK_END);
	scanf("%s", temp->email);
	printf("[INFO] %s, %s 유저가 추가되었습니다.", temp->name, temp->email);
	
	//스캔한 정보(이메일, 이름)을 유저 연결리스트에 연결한다. 
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

	//연결 후 알파벳 순으로 다시 정렬
	swap(plist, *num);
}

//user delete 명령 처리하는 함수
void user_delete(USER* plist, int* num)
{
	USER* pos;
	USER* temp;
	char name[21];//제거할 유저의 이름을 스캔할 함수
	char yn;//정말 제거하시겠습니까에서 y/n 결정

	printf("제거할 유저의 이름을 입력해주세요. >> ");
	fseek(stdin, 0, SEEK_END);
	scanf("%s", name);

	//제거할 유저를 유저 연결 리스트에서 찾는다
	for (pos = plist->next; pos != NULL; pos = pos->next)
	{
		//만약 스캔한 이름과 유저리스트가 같다면
		if (strcmp(name, pos->name) == 0)
		{
			printf("%s, %s 유저를 제거하시겠습니까? (y/n) >> ", pos->name, pos->email);
			fseek(stdin, 0, SEEK_END);
			scanf("%c", &yn);
			if (yn == 'y')
			{
				printf("[INFO] %s, %s 유저를 제거하였습니다.", pos->name, pos->email);
				//유저 수 줄어든다
				*num = *num - 1;
				//만약 입력한 수가 첫번째 node에 연결돼있다면
				if (plist->next == pos) 
				{
					plist->next = pos->next;
					free(pos);
				}
				//입력한 수가 첫번째 node가 아니라면
				else
				{
					//temp를 통해 일치하는 노드를 찾는다.
					for (temp = plist->next; temp->next != NULL; temp = temp->next)
					{
						//일치하는 노드를 찾으면 break
						if (temp->next == pos)
						{
							break;
						}
					}
					//일치하는 노드를 제거한다.
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
	 printf("[INFO] 해당 유저는 존재하지 않습니다!");
}

void swap(USER* plist, int num)
{
	USER* temp;//정보를 임시로 저장할 노드 
	//할당과 초기화
	temp = (USER*)malloc(sizeof(USER));
	strcpy(temp->name, "\0");
	strcpy(temp->email, "\0");
	temp->next = NULL;

	//swap 함수 구현
	USER* a;
	USER* b;

	//버블 정렬 구조
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
	//할당 해제
	free(temp);
}

//회의 정보
void conf_info(CONFERENCE *conf)
{
	PARTICIPANT* temp; //participant 연결 리스트를 temp를 옮기면서 출력한다
	printf("회의실 이름: %s\n",conf->roomName);
	printf("최대 사용자수: %d\n",conf->maxParticipant);
	printf("참여자 목록:\n");
	printf("================================================================================\n");
	printf("                이름                                   이메일    카메라   마이크\n");
	printf("--------------------------------------------------------------------------------\n");
	//participant 연결 리스트를 temp를 옮기면서 출력한다. 
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

//회의에 유저 넣는 함수
void conf_join(USER *plist,CONFERENCE *conf)
{	
	//participant 수가 이미 max num에 다달했을 경우
	if (conf->maxParticipant == conf->numParticipant)
	{
		printf("[INFO] 회의에 최대 인원 %d 명이 참여하고 있습니다.",conf->maxParticipant);
		return;
	}

	char name[21];//이름 20글자
	char yn;//카메라 마이크 여부 판단
	PARTICIPANT* NewParticipants;//새로운 유저 participant list 에 연결하는 변수
	PARTICIPANT* ptemp;//
	USER* utemp;//입력한 유저가 participant list 에 있는지 판단하는 포인터

	//새로운 참가자 node 할당과 초기화
	NewParticipants = (PARTICIPANT*)malloc(sizeof(PARTICIPANT));
	NewParticipants->next = NULL;
	NewParticipants->user = NULL;

	//추가할 유저의 이름 입력
	printf("추가할 유저의 이름을 입력해주세요. >> ");
	fseek(stdin, 0, SEEK_END);
	scanf("%s", name);

	//만약 유저가 없다면 없다는 메시지 출력하기
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
			printf("[INFO] 해당 유저는 존재하지 않습니다!");
			return;
		}
	}

	//입력한 이름이 이미 participant list에 있다면 이미 참여했다는 메시지 출력하기
	for (ptemp = conf->participants; ptemp != NULL; ptemp = ptemp->next)
	{
		if (strcmp(name, ptemp->user->name) == 0)
		{
			printf("[INFO] %s이가 이미 회의에 참여했습니다!", name);
			return;
		}
	}

	//카메라 마이크 on off 여부 확인 후 NewParticiapnt node에 정보 추가하기
	printf("카메라를 켠 상태로 시작하시겠습니까? (y/n) >> ");
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
	printf("마이크를 켠 상태로 시작하시겠습니까? (y/n) >> ");
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

	//새로운 참가자라면 리스트에 추가
	//첫번째 참가자라면
	if (conf->participants == NULL)
	{
		conf->participants = NewParticipants;
		conf->numParticipant++;
	}
	//두번째로 연결 리스트에 연결하는 것이라면 
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


	//user list에 입력된 사람 있는지 찾은후 participant node로 user로 가르키기
	for (utemp = plist->next; utemp != NULL; utemp = utemp->next)
	{
		//user list에서 같은 사람 찾아서 새로 생성된 participants랑 연결하기
		if (strcmp(name, utemp->name) == 0)
		{
			//먼저 구조 연결하고 카메라 입력받을 것. 
			NewParticipants->user = utemp;
			printf("[INFO] %s 이(가) 회의에 참여했습니다!",utemp->name);
			return;
		}
	}
	return;
}

// participant list 에서 유저 제거하기
void conf_hangup(CONFERENCE* conf)
{
	PARTICIPANT* pos;
	PARTICIPANT* temp;
	char name[21];
	char yn;

	printf("제외할 유저의 이름을 입력해주세요. >> ");
	fseek(stdin, 0, SEEK_END);
	scanf("%s", name);
	for (pos = conf->participants; pos != NULL; pos = pos->next)
	{
		if (strcmp(name, pos->user->name) == 0)
		{
			printf("정말로 회의에서 내보내시겠습니까? (y/n) >> ");
			fseek(stdin, 0, SEEK_END);
			scanf("%c", &yn);
			if (yn == 'y')
			{
				printf("[INFO] %s 이(가) 회의에서 나갔습니다!", name);
				//첫번째 유저일 경우 삭제하는 방법
				if (conf->participants == pos)
				{
					conf->participants = pos->next;
					free(pos);
				}
				//첫번째가 아닌 그 다음 유저일 경우 삭제하는 방법
				else
				{
					//temp를 통해 일치하는 노드를 찾는다.
					for (temp = conf->participants; temp->next != NULL; temp = temp->next)
					{
						//일치하면 break
						if (temp->next == pos)
						{
							break;
						}
					}
					//node 연결하고 free 하기
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
	printf("[INFO] 해당 유저는 회의에 존재하지 않습니다!");
}

//카메라 on off 상태 바꾸는 함수
void conf_toggle_camera(CONFERENCE* conf)
{
	char name[21];// 상태 바꾸고 싶은 유저 이름 스캔
	PARTICIPANT* temp;//conference node를 가리키면서 상태 바꾸기
	printf("카메라 상태를 변경할 유저의 이름을 입력해주세요. >> ");
	scanf("%s", name);
	for (temp = conf->participants; temp != NULL; temp = temp->next)
	{
		//만약 입력한 이름과 user list이름과 일치한다면
		if (strcmp(name, temp->user->name) == 0)
		{
			//마이크가 켜져있는 경우
			if (temp->camera == true)
			{
				temp->camera = false;
				printf("[INFO] %s의 카메라가 ON->OFF 되었습니다!", name);
			}
			//마이크가 꺼져있는 경우
			else if (temp->camera == false)
			{
				temp->camera = true;
				printf("[INFO] %s의 카메라가 OFF->ON 되었습니다!", name);
			}
			return;
		}
	}
	printf("[INFO] 해당 유저는 회의에 존재하지 않습니다!");
}

//마이크 상태 on off 하는 함수
void conf_toggle_mic(CONFERENCE* conf)
{
	char name[21];//상태를 바꾸고 싶은 유저의 이름 스캔
	PARTICIPANT* temp; // conference node를 가리키면서 상태 바꾸기
	printf("마이크 상태를 변경할 유저의 이름을 입력해주세요. >> ");
	scanf("%s", name);
	for (temp = conf->participants; temp != NULL; temp = temp->next)
	{
		//만약 입력한 이름과 user list이름과 일치한다면
		if (strcmp(name, temp->user->name) == 0)
		{
			//마이크 켜져있는 경우
			if (temp->microphone == true)
			{
				temp->microphone = false;
				printf("[INFO] %s의 마이크가 ON->OFF 되었습니다!", name);
			}
			//마이크 꺼져있는 경우
			else if (temp->microphone == false)
			{
				temp->microphone = true;
				printf("[INFO] %s의 마이크가 OFF->ON 되었습니다!", name);
			}
			return;
		}
	}
	printf("[INFO] 해당 유저는 회의에 존재하지 않습니다!");
}