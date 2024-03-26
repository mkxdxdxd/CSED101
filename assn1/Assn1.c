#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int print_menu();
int game_info();
void game_start();
void print_map(int user_pos);
char set_position(int user_pos, int pos);
void play_juru(int user_pos, int* pdrink, int* pdrink_comp, int* pcomp_drink_now);
void print_drink(int user_pos, int drink, int drink_comp);
void start(int* pdrink, int* pdrink_comp, int* pcomp_drink_now);
void normal(int* pdrink, int* pdrink_comp, int* pcomp_drink_now);
void rest(int* pdrink, int* pdrink_comp, int* pcomp_drink_now);
void chance(int* pdrink, int* pdrink_comp, int* pcomp_drink_now);
void mission(int* pdrink, int* pdrink_comp, int* pcomp_drink_now);
void mission_baskin(int* pdrink, int* pdrink_comp, int* pcomp_drink_now);
void mission_369(int* pdrink, int* pdrink_comp, int* pcomp_drink_now);
void computer_turn(int drink, int comp_drink_now);


//main 함수
//main 함수에서는 print menu를 출력하고 1. 게임 설명, 2. 게임 시작, 3. 게임 종료를 진행한다.
//반복문을 while(1)로 설정하여 다른 입력을 받았을때, 게임 설명, 게임 시작을 선택했을때 계속 반복되도록 설정하였다. 
//게임 시작을(2번)을 누르면 game_start 함수를 호출한다. 

int main()
{
	srand(time(NULL));
	//변수 a는 1번 게임 설명 함수를 실행하고 다시 메뉴로 돌아올떄 입력값을 받아오는 역할을 한다. 
	//변수 menu_num은 메뉴에서 1,2,3과 같은 입력을 받는 역할을 한다. 
	int a;
	int menu_num;
	while (1)
	{
		menu_num = print_menu();

		switch (menu_num)
		{
		case 1:
			system("cls");
			a = game_info();
			if (a == -1)
			{
				system("cls");
				continue;
			}
			break;

		case 2:
			system("cls");
			game_start();
			break;

		case 3: printf("게임을 종료합니다..."); break;

		default:
			system("cls");
			break;
		}

		if (menu_num == 3)
			break;
		else if (menu_num == 1 || menu_num == 2)
		{
			system("cls");
		}
		else if (menu_num != 1 || menu_num != 2)
		{
			system("cls");
			printf("메뉴를 다시 입력하세요.\n");
		}
	}
	return 0;
}

//print_menu 함수는 메뉴를 출력하는 역할을 하고 입력값을 받아 main 함수로 반환한다. 
int print_menu()
{
	//변수 a는 입력을 받는 역할을 한다. 
	int a;
	printf("[주루마블]\n");
	printf("=============================\n");
	printf("     1.게임 설명\n");
	printf("     2.게임 시작\n");
	printf("     3.게임 종료\n");
	printf("=============================\n");
	printf("입력:");
	scanf("%d", &a);
	return (a);
}

//game_info 함수는 print_menu 함수에서 1을 입력받았을때 실행되며, 게임에 대한 정보가 쓰여있다. 
//함수가 출력되고 다시 main 메뉴로 돌아가기 위해 -1또는 0값을 입력받고 main함수로 반환되어 값이 -1이라면 game_start 함수를 호출한다. 
int game_info()
{
	int back_to_main;
	printf("============주루마블 게임 설명============\n\n");
	printf("1. USER는 매턴 주사위를 던진다. 주사위를 던질 때 주사위의 눈은 플레이어가 결정한다.\n\n");
	printf("2. COMPUTER는 주사위를 던지지 않으며 매턴 1잔의 술을 마신다.\n\n");
	printf("3. USER와 COMPUTER의 주량은 소주 2변(14잔)이며, 14잔을 먼저 마신 쪽이 패배한다.\n\n");
	printf("4. 보드 칸은 5가지 칸으로 이루어져 있으며 USER는 각 칸의 요구사항을 만족시켜야한다.\n\n");
	printf("===========================================\n");
	printf("메인 메뉴로 돌아가기(-1):");
	scanf("%d", &back_to_main);
	return back_to_main;
}

//game_start 함수에서는 게임을 본격적으로 시작한다. 
void game_start()
{
	int roll;//주사위를 굴리시겠습니까? 에서 값을 받아오는 변수
	int dice;//주사위를 랜덤하게 굴렸을경우 1~6까지 한 숫자가 저장되는 변수
	int user_pos;//사용자의 현재 위치를 print_map으로 보내는 역할을 한다. 
	int round=0;//라운드 수를 알려주는 변수
	int drink=0;//사용자의 현재 잔 수, 0부터 시작하여 0으로 초기화한다. 
	int drink_comp=0;//COMP의 현재 잔 수, 0 부터 시작하여 0으로 초기화한다. 
	int comp_drink_now = 0;//라운드마다 현재 COMP가 마시는 잔수, 누적된 잔수와 차별화 되는 변수이며 매 round마다 값이 변하므로 변수로 설정했다. 
	int a;//-1입력시 메인함수로 돌아가기 위한 변수

	while (1)
	{
		printf("[주루마블게임]\n");
		printf("==================================\n");
		printf("+-------+-------+-------+-------+\n");
		printf("| 미션  | 찬스  | 일반  | 휴식  |\n");
		printf("+-------+-------+-------+-------+\n");
		printf("| 일반  |               | 일반  |\n");
		printf("+-------+               +-------+\n");
		printf("| 일반  |               | 일반  |\n");
		printf("+-------+-------+-------+-------+\n");
		printf("| 시작 O| 일반  | 찬스  | 미션  |\n");
		printf("+-------+-------+-------+-------+\n");
		printf("==================================\n");
		printf("[USER - 0] ( 0/14)\n");
		printf("□□□□□□□□□□□□□□\n\n");
		printf("[COMP - 0] ( 0/14)\n");
		printf("□□□□□□□□□□□□□□\n");
		printf("==================================\n\n");
		printf("주사위를 굴리시겠습니다? (0-굴림,-1-게임포기):");
		scanf("%d", &roll);

		//주사위를 굴린다면 roll에 0, 포기한다면, -1값이 저장된다. 
		//roll에 1,2,3,4,5,6값이 입력될 수 있고, dice에 roll 값이 입력된다. 

		switch (roll)
		{
		case -1:
			return;
		case 0:
			dice = rand() % 6 + 1;
			printf("%d", dice);
			break;

		case 1: case 2: case 3: case 4: case 5: case 6:
			dice = roll;
			break;

		default:
			system("cls");
			break;
		}

		if (roll == 1||roll==2||roll==3||roll==4||roll==5||roll==6||roll==0||roll==-1) break;
	}
	
	system("cls");
	user_pos = dice;

	//라운드1 부터 본격적으로 게임이 시작된다. 
	while (1)
	{
		while (1)
		{	
			//Round는 while 문이 한번 반복될때마다 1씩 올라간다. 
			round++;

			//만약 user의 현재 위치가 11이라는 숫자를 넘어갈 경우, 보드 칸이 11칸이므로 12를 빼준다. 
			if (user_pos>11)
				user_pos = user_pos - 12;

			system("cls");
			printf("==================================\n");
			printf("▶▶▶ ROUND %d▶▶▶▶▶▶▶▶▶▶\n", round);
			printf("==================================\n");
			printf("[USER's Turn]\n");
			printf("----------------------------------\n");
			printf("주사위 눈:%d\n\n", dice);//주사위의 값이 나타난다. 
			print_map(user_pos);//사용자의 현재 위치를 지도 위에 나타내준다. 
			printf("\n");
			play_juru(user_pos,&drink,&drink_comp, &comp_drink_now);//user의 위치 1~11에 따라 보트 칸에 맞는 게임을 진행한다. 
			computer_turn(drink, comp_drink_now);//computer가 현재 라운드에서 몇잔을 마셔야하는지 알려준다. 
			printf("\n================================\n");
			
			//만약 user의 주량이 14잔을 넘었을 경우 COMP가 승리했다고 해야하고
			//COMP의 주량이 14잔을 넘겼을 경우 USER가 승리했다고 프린트해야한다. 
			if (drink >= 14)
			{
				drink = 14;
				printf("\n게임이 종료되었습니다.\n");
				print_drink(user_pos, drink, drink_comp);
				printf("\n*결과 COMPUTER가 승리하였습니다\n\n");
				break;
			}
		
			else if (drink_comp >= 14)
			{
				drink_comp = 14;
				printf("\n게임이 종료되었습니다\n");
				print_drink(user_pos, drink, drink_comp);
				printf("\n*결과 USER가 승리하였습니다.\n\n");
				break;
			}

			print_drink(user_pos, drink, drink_comp);//누적 주량 프린트
			printf("\n================================\n\n");
			printf("주사위를 굴리시겠습니다? (0-굴림,-1-게임포기):");
			scanf("%d", &roll);

			switch (roll)
			{
			case -1:
				return;
			case 0:
				dice = rand() % 6 + 1;
				printf("%d", dice);
				break;

			case 1: case 2: case 3: case 4: case 5: case 6:
				dice = roll; break;

			default:
				system("cls"); break;
		
			}
			user_pos = user_pos + dice;

			if (roll == 1 || roll == 2 || roll == 3 || roll == 4 || roll == 5 || roll == 6 || roll == 0 || roll == -1) break;
		}

		if (drink >= 14||drink_comp >= 14) break;
	}

	//게임이 종료된 후 메인 메뉴로 돌아간다. 
	printf("메인 메뉴로 돌아가기(-1)\n");
	scanf("%d", &a);
	if (a == -1) {
		return;
	}
}

//사용자의 현재 위치를 알려주는 함수
void print_map(int user_pos)
{
		printf("+-------+-------+-------+-------+\n");
		printf("| 미션 %c| 찬스 %c| 일반 %c| 휴식 %c|\n", set_position(user_pos, 9), set_position(user_pos, 8), set_position(user_pos, 7), set_position(user_pos, 6));
		printf("+-------+-------+-------+-------+\n");
		printf("| 일반 %c|               | 일반 %c|\n", set_position(user_pos, 10), set_position(user_pos, 5));
		printf("+-------+               +-------+\n");
		printf("| 일반 %c|               | 일반 %c|\n", set_position(user_pos, 11), set_position(user_pos, 4));
		printf("+-------+-------+-------+-------+\n");
		printf("| 시작 %c| 일반 %c| 찬스 %c| 미션 %c|\n", set_position(user_pos, 0), set_position(user_pos, 1), set_position(user_pos, 2), set_position(user_pos, 3));
		printf("+-------+-------+-------+-------+\n");
		
}

//현재 위치를 받아와서 현재 위치와 포지션 숫자가 일치한다면 O을 반환하는 함수
char set_position(int user_pos, int pos)
{
	if (user_pos == pos) return 'O';
	else if (user_pos != pos) return ' ';
}

//play_juru 함수에서는 주량을 각 칸수 1~11에 맞게 게임을 진행한다. Switch 구문을 사용한다. 
//또한 포인터 변수가 사용된 것을 알 수 있는데, game_start로부터 drink, drink_comp&comp_drink_now의 주소를 가져와서 그 주소에 값을 입력한다. 
void play_juru(int user_pos, int *pdrink, int *pdrink_comp, int *pcomp_drink_now)
{	
	switch (user_pos)
	{
	case 1: case 4: case 5: case 7: case 10: case 11:
		normal(pdrink, pdrink_comp, pcomp_drink_now); break;
	case 2: case 8:
		chance(pdrink, pdrink_comp, pcomp_drink_now); break;
	case 3:case 9:
		mission(pdrink, pdrink_comp, pcomp_drink_now); break;
	case 6:
		rest(pdrink, pdrink_comp, pcomp_drink_now); break;
	case 0:
		start(pdrink, pdrink_comp, pcomp_drink_now); break;
	}
}

//print 함수에서는 user_pos, drink & drink_comp 값을 입력받아 누적 주량을 프린트한다. 
void print_drink(int user_pos, int drink, int drink_comp)
{
	int i;
	printf("[USER - %d] (%2d/14)\n", user_pos, drink);
	//USER 의 경우 검은 네모는 현재 주량만큼 뽑는 것을 반복하고
	//14개중 나머지는 흰 네모를 프린트한다. 
	for (i = 1; i <= 14; i++) 
	{
		if (drink >= i) printf("■");
		else printf("□");
	}

	printf("\n\n[COMP - 0] (%2d/14)\n", drink_comp);
	for (i = 1; i <= 14; i++)
	{
		if (drink_comp >= i) printf("■");
		else printf("□");
	}

}

//시작 칸에서는 컴퓨터만 1잔 마신다. 
void start(int *pdrink, int*pdrink_comp, int *pcomp_drink_now)
{
	printf("시작 칸에 도착하였습니다.\n");
	printf("이번 턴은 아무것도 하지 않습니다.\n\n");
	*pcomp_drink_now = 1;
	if (*pdrink<14) (*pdrink_comp)++;
}

//일반칸에서는 유저와 컴퓨터 모두 1잔씩 마신다. 
void normal(int* pdrink, int* pdrink_comp, int *pcomp_drink_now)
{
	//일반칸, USER 한잔 더 마심
	printf("일반 칸에 도착하였습니다.\nUSER가 1잔 마시게 되었습니다.\n\n");
	*pcomp_drink_now = 1;
	(*pdrink)++;
	if (*pdrink < 14) (*pdrink_comp)++;
}

//휴식 칸에서는 컴퓨터만 1잔 마신다. 
void rest(int* pdrink, int* pdrink_comp, int *pcomp_drink_now)
{
	printf("휴식칸에 도착하였습니다. 이번 턴은 아무것도 하지 않습니다.\n\n");
	*pcomp_drink_now = 1;
	if (*pdrink < 14) (*pdrink_comp)++;
}

//찬스 칸에서는 세잔 더와 여명 808 중 랜덤으로 선택해서 실행
void chance(int* pdrink, int* pdrink_comp, int* pcomp_drink_now)
{
	int chance_type;
	printf("찬스 칸에 도착하였습니다.\n");
	chance_type = rand() % 2;

	if (chance_type == 0)
	{
		printf("'세잔 더!' 카드를 뽑았습니다.\nUSER가 3잔 마시게 되었습니다.\n\n");
		*pcomp_drink_now = 1;

		(*pdrink) += 3;
		if (*pdrink < 14) (*pdrink_comp)++;
	}

	else if (chance_type == 1)
	{
		printf("'여명 808' 카드를 뽑았습니다.\nUSER가 -2잔 마시게 되었습니다.\n\n");
		*pcomp_drink_now = 1;
		if (*pdrink <= 1)
		{
			*pdrink = 0;
			if (*pdrink < 14) (*pdrink_comp)++;
		}
		else
		{
			(*pdrink) -= 2;
			if (*pdrink < 14) (*pdrink_comp)++;
		}
	}
}


//미션칸 369 게임이나 베스킨라빈스 게임중 랜덤으로 선택하여 실행
void mission(int *pdrink, int *pdrink_comp,int* pcomp_drink_now)
{
	int mission_type;
	printf("미션칸에 도착하였습니다.\n");
	mission_type = rand() % 2;

	if (mission_type == 0)
	{
		mission_baskin(pdrink,pdrink_comp,pcomp_drink_now);
	}

	else if (mission_type == 1)
	{
		mission_369(pdrink, pdrink_comp,pcomp_drink_now);
	}
}

//베스킨라빈스 함수를 실행한다. 
void mission_baskin(int *pdrink, int *pdrink_comp, int* pcomp_drink_now)
{
	int turn; //turn이 0으로 배정될 경우 user 먼저 실행, 1일 경우 comp 부터 실행
	int n1 = 0, n2 = 0; //USER로부터 입력받는 수
	int num = 0;//COMP를가 출력하는 값

	printf("베스킨라빈스 게임이 선택되었습니다.\n");
	printf("-------------------------------------\n");
	printf("★★★★★베스킨라빈스 게임★★★★★\n");
	printf("-------------------------------------\n");

	turn = rand() % 2;

	if (turn == 0)
	{
		while (1)
		{
			//USER 차례
			printf("USER >> ");
			scanf("%d", &n1);

			while (1)
			{
				//입력값이 3보다 큰 수이거나 0보다 작을 경우 WRONG INPUT 이라는 메시지를 출력하고 번호를 다시 입력받는다. 
				if (n1 > 3 || n1 <= 0)
				{
					printf("WRONG INPUT!");
					printf("\nUSER >> ");
					scanf("%d", &n1);
				}
				else
					break;
			}
			
			//num 이라는 수를 1부터 정해서 입력받은 n1수 만큼 반복해서 프린트한다.
			//n1 이라는 수는 1,2,3중 하나일 것이고 num은 n1만큼 반복되어 출력된다. 
			for (int i = 1; i <= n1; i++)
			{
				num++;
				if (num <= 31)
				{
					printf("%d ", num);
				}
			}

			//USER 차례에 31을 넘는 수를 말했다면 컴퓨터 승리
			if (num >= 31)
			{
				printf("\nCOMPUTER가 승리하였습니다.\nUSER가 4잔 마시게 되었습니다.\n");
				*pcomp_drink_now = 0;
				if (*pdrink < 14) (*pdrink) += 4;
				break;
			}

			//컴퓨터 차례
			n2 = (rand() % 3 + 1);
			printf("\nCOMP >> %d\n", n2);

			for (int i = 0; i < n2; i++)
			{
				num++;
				if (num <= 31)
				{
					printf("%d ", num);
				}
			}
			printf("\n");

			//COMPUTER 차례에 31을 넘는 수를 말했다면 USER 승리
			if (num >= 31)
			{
				printf("\nUSER가 승리하였습니다.\nCOMPUTER가 4잔 마시게 되었습니다.\n\n");
				*pcomp_drink_now = 0;
				if (*pdrink < 14) (*pdrink_comp) += 4;
				break;
			}
		}
	}

	else if (turn == 1)
	{
		while (1)
		{
			//컴퓨터 차례
			n2 = (rand() % 3 + 1);
			printf("COMP >> %d\n", n2);

			for (int i = 0; i < n2; i++)
			{
				num++;
				if (num <= 31)
				{
					printf("%d ", num);
				}
			}
			printf("\n");

			//COMPUTER 차례에 31을 넘는 수를 말했다면 USER 승리
			if (num >= 31)
			{
				printf("\nUSER가 승리하였습니다.\nCOMP가 4잔 마시게 되었습니다.\n\n");
				*pcomp_drink_now = 0;
				if (*pdrink < 14) (*pdrink_comp) +=4;
				break;
			}

			//USER 차례
			printf("USER >> ");
			scanf("%d", &n1);

			while (1) 
			{
				if (n1 > 3 || n1 <= 0)
				{
					printf("WRONG INPUT!");
					printf("\nUSER >> ");
					scanf("%d", &n1);
				}
				else
					break;
			}

			for (int i = 1; i <= n1; i++)
			{
				num++;
				if (num <= 31)
				{
					printf("%d ", num);
				}
			}

			printf("\n");

			//USER 차례에 31을 넘는 수를 말했다면 COMPUTER 승리
			if (num >= 31)
			{
				printf("\nCOMPUTER가 승리하였습니다.\nUSER가 4잔 마시게 되었습니다.\n\n");
				*pcomp_drink_now = 0;
				if (*pdrink < 14) (*pdrink) += 4;
				break;
			}
		}
	}
}


//369 미션을 실행한다.
void mission_369(int* pdrink, int* pdrink_comp, int* pcomp_drink_now)
{
	int turn;//turn 값에 따라 선 공격자가 정해진다.
	int num1;//USER가 먼저일때, 초기 입력값을 받는 변수
	int num2;//USER가 먼저일떄, 입력값을 받는 변수
	int num_comp=1,num = 1;//COMP 출력값과 비교하는 기준이 되는 변수
	int remainder, remainder2;//3,6,9,0이 들어간 수를 찾는 변수
	int num_comp2;//COMP가 먼저일때, 입력값을 받는 변수

	printf("369 게임이 선택되었습니다.\n");
	printf("--------------------------------\n");
	printf("★★★★★  369게임  ★★★★★\n");
	printf("--------------------------------\n");

	turn = rand() % 2;

	//USER 먼저 시작
	if (turn == 0)
	{
		while (1)
		{
			printf("USER >> ");
			scanf("%d", &num1);

			if (!(num1 == 1))//입력값이 1이 아닌 수라면 break
				break;
			else 
				num++;
			printf("COMP >> ");
			printf("%d", 2);
			printf("\n");

			while (1)
			{
				num++;
				printf("USER >> ");//USER의 두번쨰 입력
				scanf("%d", &num2);
				remainder = num % 10;//num 기준값에 나머지를 계산하여 3,5,9,0이 들어간 숫자를 분류한다. 

				if ((num != num2) && num2 != 0)//기준값이 num(매 턴마다 1씩 증가하는 수)과 입력값인 num2가 일치하지 않을경우 break
					break;
				else if (num2 >= 21)//입력값이 21일경우 user의 승리이므로 break
					break;
				else if ((remainder == 3 || remainder == 6 || remainder == 9 || remainder == 0)&&num2!=0)//0을 0,3,6,9턴이 아닌 아무때나 쳤을 경우 break
					break;
				else if (num2 == 0&&!(remainder == 3 || remainder == 6 || remainder == 9 || remainder == 0))//0을 0,3,6,9턴이 아닌 아무때나 쳤을 경우 break
					break;
				else
				{
					num++;
					switch (num)//기준값이 3,6,9,0이 들어가는 경우 COMP는 0을 말해야 하고, 나머지 경우 그냥 num 기준값 말해준다. 
					{
					case 3:case 6:case 9:case 10:case 13:case 16:case 19:case 20:
						printf("COMP >> ");
						printf("%d", 0);
						printf("\n");
						break;

					default:
						printf("COMP >> ");
						printf("%d", num);
						printf("\n");
						break;
					}

				}

			}
			if ((num != num2) && num2 != 0)
				break;
			else if ((remainder == 3 || remainder == 6 || remainder == 9 || remainder == 0) && num2 != 0)
				break;
			else if (num2 == 0 && !(remainder == 3 || remainder == 6 || remainder == 9 || remainder == 0))
				break;
			else if (num2 >= 21)
				break;
		}
	
		printf("--------------------------------\n");

		if (num == 21)
		{
			printf("USER가 승리하였습니다.\n");
			printf("COMPUTER가 4잔 마시게 되었습니다.\n\n");
			*pcomp_drink_now = 0;
			if (*pdrink < 14) (*pdrink_comp) += 4;
		}
		else
		{
			printf("COMPUTER가 승리하였습니다.\n");
			printf("USER가 4잔 마시게 되었습니다\n\n");
			*pcomp_drink_now = 0;
			if (*pdrink < 14) (*pdrink) += 4;
		}
	}

	//COMPUTER 먼저 시작
	else if (turn == 1)
	{
		printf("COMP >> %d\n", 1);
		num_comp++;

		while (1) {
			printf("USER >> ");
			scanf("%d", &num_comp2);
			remainder2 = num_comp % 10;

			if ((num_comp != num_comp2) && num_comp2 != 0)
				break;
			else if ((((remainder2 == 3 || remainder2 == 6) || remainder2 == 9) || remainder2 == 0) && num_comp2 != 0)
				break;
			else if (num_comp2 == 0 && !(remainder2 == 3 || remainder2 == 6 || remainder2 == 9 || remainder2 == 0))
				break;
			else 
			{
				num_comp++;
				switch (num_comp)
				{
				case 3: case 6: case 9: case 10: case 13: case 16: case 19: case 20:
					printf("COMP >> ");
					printf("%d", 0);
					printf("\n");
					break;

				default:
					printf("COMP >> ");
					printf("%d", num_comp);
					printf("\n");
					break;
				}
				num_comp++;
			}

			if (num_comp >= 21)
				break;
		}
		printf("--------------------------------\n");
		if (num_comp >= 21)
		{
			printf("USER가 승리하였습니다.\n");
			printf("COMPUTER가 4잔 마시게 되었습니다.\n\n");
			*pcomp_drink_now = 0;
			if (*pdrink < 14) (*pdrink_comp) += 4;
		}
		else
		{
			printf("COMPUTER가 승리하였습니다.\n");
			printf("USER가 4잔 마시게 되었습니다.\n\n");
			*pcomp_drink_now = 0;
			if (*pdrink < 14) (*pdrink) += 4;
		}
	}
}

//컴퓨터가 라운드마다 몇잔을 마시는지 알려주는 함수.
void computer_turn(int drink, int comp_drink_now)
{
	if (drink < 14)//조건문을 달아 drink가 14일때 이 문구가 실행되지 않게 한다. 
	{
		printf("[COMPUTER's TURN]\n");
		printf("--------------------------------\n");
		printf("COMPUTER가 %d잔 마시게 되었습니다.\n",comp_drink_now);
	}
}