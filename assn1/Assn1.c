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


//main �Լ�
//main �Լ������� print menu�� ����ϰ� 1. ���� ����, 2. ���� ����, 3. ���� ���Ḧ �����Ѵ�.
//�ݺ����� while(1)�� �����Ͽ� �ٸ� �Է��� �޾�����, ���� ����, ���� ������ ���������� ��� �ݺ��ǵ��� �����Ͽ���. 
//���� ������(2��)�� ������ game_start �Լ��� ȣ���Ѵ�. 

int main()
{
	srand(time(NULL));
	//���� a�� 1�� ���� ���� �Լ��� �����ϰ� �ٽ� �޴��� ���ƿË� �Է°��� �޾ƿ��� ������ �Ѵ�. 
	//���� menu_num�� �޴����� 1,2,3�� ���� �Է��� �޴� ������ �Ѵ�. 
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

		case 3: printf("������ �����մϴ�..."); break;

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
			printf("�޴��� �ٽ� �Է��ϼ���.\n");
		}
	}
	return 0;
}

//print_menu �Լ��� �޴��� ����ϴ� ������ �ϰ� �Է°��� �޾� main �Լ��� ��ȯ�Ѵ�. 
int print_menu()
{
	//���� a�� �Է��� �޴� ������ �Ѵ�. 
	int a;
	printf("[�ַ縶��]\n");
	printf("=============================\n");
	printf("     1.���� ����\n");
	printf("     2.���� ����\n");
	printf("     3.���� ����\n");
	printf("=============================\n");
	printf("�Է�:");
	scanf("%d", &a);
	return (a);
}

//game_info �Լ��� print_menu �Լ����� 1�� �Է¹޾����� ����Ǹ�, ���ӿ� ���� ������ �����ִ�. 
//�Լ��� ��µǰ� �ٽ� main �޴��� ���ư��� ���� -1�Ǵ� 0���� �Է¹ް� main�Լ��� ��ȯ�Ǿ� ���� -1�̶�� game_start �Լ��� ȣ���Ѵ�. 
int game_info()
{
	int back_to_main;
	printf("============�ַ縶�� ���� ����============\n\n");
	printf("1. USER�� ���� �ֻ����� ������. �ֻ����� ���� �� �ֻ����� ���� �÷��̾ �����Ѵ�.\n\n");
	printf("2. COMPUTER�� �ֻ����� ������ ������ ���� 1���� ���� ���Ŵ�.\n\n");
	printf("3. USER�� COMPUTER�� �ַ��� ���� 2��(14��)�̸�, 14���� ���� ���� ���� �й��Ѵ�.\n\n");
	printf("4. ���� ĭ�� 5���� ĭ���� �̷���� ������ USER�� �� ĭ�� �䱸������ �������Ѿ��Ѵ�.\n\n");
	printf("===========================================\n");
	printf("���� �޴��� ���ư���(-1):");
	scanf("%d", &back_to_main);
	return back_to_main;
}

//game_start �Լ������� ������ ���������� �����Ѵ�. 
void game_start()
{
	int roll;//�ֻ����� �����ðڽ��ϱ�? ���� ���� �޾ƿ��� ����
	int dice;//�ֻ����� �����ϰ� ��������� 1~6���� �� ���ڰ� ����Ǵ� ����
	int user_pos;//������� ���� ��ġ�� print_map���� ������ ������ �Ѵ�. 
	int round=0;//���� ���� �˷��ִ� ����
	int drink=0;//������� ���� �� ��, 0���� �����Ͽ� 0���� �ʱ�ȭ�Ѵ�. 
	int drink_comp=0;//COMP�� ���� �� ��, 0 ���� �����Ͽ� 0���� �ʱ�ȭ�Ѵ�. 
	int comp_drink_now = 0;//���帶�� ���� COMP�� ���ô� �ܼ�, ������ �ܼ��� ����ȭ �Ǵ� �����̸� �� round���� ���� ���ϹǷ� ������ �����ߴ�. 
	int a;//-1�Է½� �����Լ��� ���ư��� ���� ����

	while (1)
	{
		printf("[�ַ縶�����]\n");
		printf("==================================\n");
		printf("+-------+-------+-------+-------+\n");
		printf("| �̼�  | ����  | �Ϲ�  | �޽�  |\n");
		printf("+-------+-------+-------+-------+\n");
		printf("| �Ϲ�  |               | �Ϲ�  |\n");
		printf("+-------+               +-------+\n");
		printf("| �Ϲ�  |               | �Ϲ�  |\n");
		printf("+-------+-------+-------+-------+\n");
		printf("| ���� O| �Ϲ�  | ����  | �̼�  |\n");
		printf("+-------+-------+-------+-------+\n");
		printf("==================================\n");
		printf("[USER - 0] ( 0/14)\n");
		printf("���������������\n\n");
		printf("[COMP - 0] ( 0/14)\n");
		printf("���������������\n");
		printf("==================================\n\n");
		printf("�ֻ����� �����ðڽ��ϴ�? (0-����,-1-��������):");
		scanf("%d", &roll);

		//�ֻ����� �����ٸ� roll�� 0, �����Ѵٸ�, -1���� ����ȴ�. 
		//roll�� 1,2,3,4,5,6���� �Էµ� �� �ְ�, dice�� roll ���� �Էµȴ�. 

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

	//����1 ���� ���������� ������ ���۵ȴ�. 
	while (1)
	{
		while (1)
		{	
			//Round�� while ���� �ѹ� �ݺ��ɶ����� 1�� �ö󰣴�. 
			round++;

			//���� user�� ���� ��ġ�� 11�̶�� ���ڸ� �Ѿ ���, ���� ĭ�� 11ĭ�̹Ƿ� 12�� ���ش�. 
			if (user_pos>11)
				user_pos = user_pos - 12;

			system("cls");
			printf("==================================\n");
			printf("������ ROUND %d��������������������\n", round);
			printf("==================================\n");
			printf("[USER's Turn]\n");
			printf("----------------------------------\n");
			printf("�ֻ��� ��:%d\n\n", dice);//�ֻ����� ���� ��Ÿ����. 
			print_map(user_pos);//������� ���� ��ġ�� ���� ���� ��Ÿ���ش�. 
			printf("\n");
			play_juru(user_pos,&drink,&drink_comp, &comp_drink_now);//user�� ��ġ 1~11�� ���� ��Ʈ ĭ�� �´� ������ �����Ѵ�. 
			computer_turn(drink, comp_drink_now);//computer�� ���� ���忡�� ������ ���ž��ϴ��� �˷��ش�. 
			printf("\n================================\n");
			
			//���� user�� �ַ��� 14���� �Ѿ��� ��� COMP�� �¸��ߴٰ� �ؾ��ϰ�
			//COMP�� �ַ��� 14���� �Ѱ��� ��� USER�� �¸��ߴٰ� ����Ʈ�ؾ��Ѵ�. 
			if (drink >= 14)
			{
				drink = 14;
				printf("\n������ ����Ǿ����ϴ�.\n");
				print_drink(user_pos, drink, drink_comp);
				printf("\n*��� COMPUTER�� �¸��Ͽ����ϴ�\n\n");
				break;
			}
		
			else if (drink_comp >= 14)
			{
				drink_comp = 14;
				printf("\n������ ����Ǿ����ϴ�\n");
				print_drink(user_pos, drink, drink_comp);
				printf("\n*��� USER�� �¸��Ͽ����ϴ�.\n\n");
				break;
			}

			print_drink(user_pos, drink, drink_comp);//���� �ַ� ����Ʈ
			printf("\n================================\n\n");
			printf("�ֻ����� �����ðڽ��ϴ�? (0-����,-1-��������):");
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

	//������ ����� �� ���� �޴��� ���ư���. 
	printf("���� �޴��� ���ư���(-1)\n");
	scanf("%d", &a);
	if (a == -1) {
		return;
	}
}

//������� ���� ��ġ�� �˷��ִ� �Լ�
void print_map(int user_pos)
{
		printf("+-------+-------+-------+-------+\n");
		printf("| �̼� %c| ���� %c| �Ϲ� %c| �޽� %c|\n", set_position(user_pos, 9), set_position(user_pos, 8), set_position(user_pos, 7), set_position(user_pos, 6));
		printf("+-------+-------+-------+-------+\n");
		printf("| �Ϲ� %c|               | �Ϲ� %c|\n", set_position(user_pos, 10), set_position(user_pos, 5));
		printf("+-------+               +-------+\n");
		printf("| �Ϲ� %c|               | �Ϲ� %c|\n", set_position(user_pos, 11), set_position(user_pos, 4));
		printf("+-------+-------+-------+-------+\n");
		printf("| ���� %c| �Ϲ� %c| ���� %c| �̼� %c|\n", set_position(user_pos, 0), set_position(user_pos, 1), set_position(user_pos, 2), set_position(user_pos, 3));
		printf("+-------+-------+-------+-------+\n");
		
}

//���� ��ġ�� �޾ƿͼ� ���� ��ġ�� ������ ���ڰ� ��ġ�Ѵٸ� O�� ��ȯ�ϴ� �Լ�
char set_position(int user_pos, int pos)
{
	if (user_pos == pos) return 'O';
	else if (user_pos != pos) return ' ';
}

//play_juru �Լ������� �ַ��� �� ĭ�� 1~11�� �°� ������ �����Ѵ�. Switch ������ ����Ѵ�. 
//���� ������ ������ ���� ���� �� �� �ִµ�, game_start�κ��� drink, drink_comp&comp_drink_now�� �ּҸ� �����ͼ� �� �ּҿ� ���� �Է��Ѵ�. 
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

//print �Լ������� user_pos, drink & drink_comp ���� �Է¹޾� ���� �ַ��� ����Ʈ�Ѵ�. 
void print_drink(int user_pos, int drink, int drink_comp)
{
	int i;
	printf("[USER - %d] (%2d/14)\n", user_pos, drink);
	//USER �� ��� ���� �׸�� ���� �ַ���ŭ �̴� ���� �ݺ��ϰ�
	//14���� �������� �� �׸� ����Ʈ�Ѵ�. 
	for (i = 1; i <= 14; i++) 
	{
		if (drink >= i) printf("��");
		else printf("��");
	}

	printf("\n\n[COMP - 0] (%2d/14)\n", drink_comp);
	for (i = 1; i <= 14; i++)
	{
		if (drink_comp >= i) printf("��");
		else printf("��");
	}

}

//���� ĭ������ ��ǻ�͸� 1�� ���Ŵ�. 
void start(int *pdrink, int*pdrink_comp, int *pcomp_drink_now)
{
	printf("���� ĭ�� �����Ͽ����ϴ�.\n");
	printf("�̹� ���� �ƹ��͵� ���� �ʽ��ϴ�.\n\n");
	*pcomp_drink_now = 1;
	if (*pdrink<14) (*pdrink_comp)++;
}

//�Ϲ�ĭ������ ������ ��ǻ�� ��� 1�ܾ� ���Ŵ�. 
void normal(int* pdrink, int* pdrink_comp, int *pcomp_drink_now)
{
	//�Ϲ�ĭ, USER ���� �� ����
	printf("�Ϲ� ĭ�� �����Ͽ����ϴ�.\nUSER�� 1�� ���ð� �Ǿ����ϴ�.\n\n");
	*pcomp_drink_now = 1;
	(*pdrink)++;
	if (*pdrink < 14) (*pdrink_comp)++;
}

//�޽� ĭ������ ��ǻ�͸� 1�� ���Ŵ�. 
void rest(int* pdrink, int* pdrink_comp, int *pcomp_drink_now)
{
	printf("�޽�ĭ�� �����Ͽ����ϴ�. �̹� ���� �ƹ��͵� ���� �ʽ��ϴ�.\n\n");
	*pcomp_drink_now = 1;
	if (*pdrink < 14) (*pdrink_comp)++;
}

//���� ĭ������ ���� ���� ���� 808 �� �������� �����ؼ� ����
void chance(int* pdrink, int* pdrink_comp, int* pcomp_drink_now)
{
	int chance_type;
	printf("���� ĭ�� �����Ͽ����ϴ�.\n");
	chance_type = rand() % 2;

	if (chance_type == 0)
	{
		printf("'���� ��!' ī�带 �̾ҽ��ϴ�.\nUSER�� 3�� ���ð� �Ǿ����ϴ�.\n\n");
		*pcomp_drink_now = 1;

		(*pdrink) += 3;
		if (*pdrink < 14) (*pdrink_comp)++;
	}

	else if (chance_type == 1)
	{
		printf("'���� 808' ī�带 �̾ҽ��ϴ�.\nUSER�� -2�� ���ð� �Ǿ����ϴ�.\n\n");
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


//�̼�ĭ 369 �����̳� ����Ų��� ������ �������� �����Ͽ� ����
void mission(int *pdrink, int *pdrink_comp,int* pcomp_drink_now)
{
	int mission_type;
	printf("�̼�ĭ�� �����Ͽ����ϴ�.\n");
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

//����Ų��� �Լ��� �����Ѵ�. 
void mission_baskin(int *pdrink, int *pdrink_comp, int* pcomp_drink_now)
{
	int turn; //turn�� 0���� ������ ��� user ���� ����, 1�� ��� comp ���� ����
	int n1 = 0, n2 = 0; //USER�κ��� �Է¹޴� ��
	int num = 0;//COMP���� ����ϴ� ��

	printf("����Ų��� ������ ���õǾ����ϴ�.\n");
	printf("-------------------------------------\n");
	printf("�ڡڡڡڡں���Ų��� ���ӡڡڡڡڡ�\n");
	printf("-------------------------------------\n");

	turn = rand() % 2;

	if (turn == 0)
	{
		while (1)
		{
			//USER ����
			printf("USER >> ");
			scanf("%d", &n1);

			while (1)
			{
				//�Է°��� 3���� ū ���̰ų� 0���� ���� ��� WRONG INPUT �̶�� �޽����� ����ϰ� ��ȣ�� �ٽ� �Է¹޴´�. 
				if (n1 > 3 || n1 <= 0)
				{
					printf("WRONG INPUT!");
					printf("\nUSER >> ");
					scanf("%d", &n1);
				}
				else
					break;
			}
			
			//num �̶�� ���� 1���� ���ؼ� �Է¹��� n1�� ��ŭ �ݺ��ؼ� ����Ʈ�Ѵ�.
			//n1 �̶�� ���� 1,2,3�� �ϳ��� ���̰� num�� n1��ŭ �ݺ��Ǿ� ��µȴ�. 
			for (int i = 1; i <= n1; i++)
			{
				num++;
				if (num <= 31)
				{
					printf("%d ", num);
				}
			}

			//USER ���ʿ� 31�� �Ѵ� ���� ���ߴٸ� ��ǻ�� �¸�
			if (num >= 31)
			{
				printf("\nCOMPUTER�� �¸��Ͽ����ϴ�.\nUSER�� 4�� ���ð� �Ǿ����ϴ�.\n");
				*pcomp_drink_now = 0;
				if (*pdrink < 14) (*pdrink) += 4;
				break;
			}

			//��ǻ�� ����
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

			//COMPUTER ���ʿ� 31�� �Ѵ� ���� ���ߴٸ� USER �¸�
			if (num >= 31)
			{
				printf("\nUSER�� �¸��Ͽ����ϴ�.\nCOMPUTER�� 4�� ���ð� �Ǿ����ϴ�.\n\n");
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
			//��ǻ�� ����
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

			//COMPUTER ���ʿ� 31�� �Ѵ� ���� ���ߴٸ� USER �¸�
			if (num >= 31)
			{
				printf("\nUSER�� �¸��Ͽ����ϴ�.\nCOMP�� 4�� ���ð� �Ǿ����ϴ�.\n\n");
				*pcomp_drink_now = 0;
				if (*pdrink < 14) (*pdrink_comp) +=4;
				break;
			}

			//USER ����
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

			//USER ���ʿ� 31�� �Ѵ� ���� ���ߴٸ� COMPUTER �¸�
			if (num >= 31)
			{
				printf("\nCOMPUTER�� �¸��Ͽ����ϴ�.\nUSER�� 4�� ���ð� �Ǿ����ϴ�.\n\n");
				*pcomp_drink_now = 0;
				if (*pdrink < 14) (*pdrink) += 4;
				break;
			}
		}
	}
}


//369 �̼��� �����Ѵ�.
void mission_369(int* pdrink, int* pdrink_comp, int* pcomp_drink_now)
{
	int turn;//turn ���� ���� �� �����ڰ� ��������.
	int num1;//USER�� �����϶�, �ʱ� �Է°��� �޴� ����
	int num2;//USER�� �����ϋ�, �Է°��� �޴� ����
	int num_comp=1,num = 1;//COMP ��°��� ���ϴ� ������ �Ǵ� ����
	int remainder, remainder2;//3,6,9,0�� �� ���� ã�� ����
	int num_comp2;//COMP�� �����϶�, �Է°��� �޴� ����

	printf("369 ������ ���õǾ����ϴ�.\n");
	printf("--------------------------------\n");
	printf("�ڡڡڡڡ�  369����  �ڡڡڡڡ�\n");
	printf("--------------------------------\n");

	turn = rand() % 2;

	//USER ���� ����
	if (turn == 0)
	{
		while (1)
		{
			printf("USER >> ");
			scanf("%d", &num1);

			if (!(num1 == 1))//�Է°��� 1�� �ƴ� ����� break
				break;
			else 
				num++;
			printf("COMP >> ");
			printf("%d", 2);
			printf("\n");

			while (1)
			{
				num++;
				printf("USER >> ");//USER�� �ι��� �Է�
				scanf("%d", &num2);
				remainder = num % 10;//num ���ذ��� �������� ����Ͽ� 3,5,9,0�� �� ���ڸ� �з��Ѵ�. 

				if ((num != num2) && num2 != 0)//���ذ��� num(�� �ϸ��� 1�� �����ϴ� ��)�� �Է°��� num2�� ��ġ���� ������� break
					break;
				else if (num2 >= 21)//�Է°��� 21�ϰ�� user�� �¸��̹Ƿ� break
					break;
				else if ((remainder == 3 || remainder == 6 || remainder == 9 || remainder == 0)&&num2!=0)//0�� 0,3,6,9���� �ƴ� �ƹ����� ���� ��� break
					break;
				else if (num2 == 0&&!(remainder == 3 || remainder == 6 || remainder == 9 || remainder == 0))//0�� 0,3,6,9���� �ƴ� �ƹ����� ���� ��� break
					break;
				else
				{
					num++;
					switch (num)//���ذ��� 3,6,9,0�� ���� ��� COMP�� 0�� ���ؾ� �ϰ�, ������ ��� �׳� num ���ذ� �����ش�. 
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
			printf("USER�� �¸��Ͽ����ϴ�.\n");
			printf("COMPUTER�� 4�� ���ð� �Ǿ����ϴ�.\n\n");
			*pcomp_drink_now = 0;
			if (*pdrink < 14) (*pdrink_comp) += 4;
		}
		else
		{
			printf("COMPUTER�� �¸��Ͽ����ϴ�.\n");
			printf("USER�� 4�� ���ð� �Ǿ����ϴ�\n\n");
			*pcomp_drink_now = 0;
			if (*pdrink < 14) (*pdrink) += 4;
		}
	}

	//COMPUTER ���� ����
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
			printf("USER�� �¸��Ͽ����ϴ�.\n");
			printf("COMPUTER�� 4�� ���ð� �Ǿ����ϴ�.\n\n");
			*pcomp_drink_now = 0;
			if (*pdrink < 14) (*pdrink_comp) += 4;
		}
		else
		{
			printf("COMPUTER�� �¸��Ͽ����ϴ�.\n");
			printf("USER�� 4�� ���ð� �Ǿ����ϴ�.\n\n");
			*pcomp_drink_now = 0;
			if (*pdrink < 14) (*pdrink) += 4;
		}
	}
}

//��ǻ�Ͱ� ���帶�� ������ ���ô��� �˷��ִ� �Լ�.
void computer_turn(int drink, int comp_drink_now)
{
	if (drink < 14)//���ǹ��� �޾� drink�� 14�϶� �� ������ ������� �ʰ� �Ѵ�. 
	{
		printf("[COMPUTER's TURN]\n");
		printf("--------------------------------\n");
		printf("COMPUTER�� %d�� ���ð� �Ǿ����ϴ�.\n",comp_drink_now);
	}
}