#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BOARD_SIZE 6
#include <windows.h>

void print_board();//���� ȭ���� ����ϴ� �Լ� �̰����� ���� �ٲٰ� ���� ó���Ѵ�. 
int check_position(int i1, int j1, int i2, int j2);//�Է¹��� ��ġ�� ��ȿ����, ���� ��ġ�ϴ��� üũ�Ѵ�. 
void set_color_red();//���� �������� �ٲٱ� ���� �Լ�
void reset_color();//���� �������� �ٲٱ� ���� �Լ�
int score(int arrb[][BOARD_SIZE], int total_score);//������ �����ؼ� ���ϴ� �Լ�
int match_block(int arrb[][BOARD_SIZE],int arr[][BOARD_SIZE]);//�����ؼ� 3�� �̻��� ���� ��ġ�� ��� ó���ϴ� �Լ�

int main()
{//���ӽ���ȭ�� ���
	char enter;
	srand(time(NULL));
	printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("CSED 101\n");
	printf("Assignment 2\n\n");
	printf("[[  Match-3 Game   ]]\n\n");
	printf("Press [Enter] to start\n");
	printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	scanf("%c", &enter);//enter���� �Է��� ������� �����Ƿ� ó������ ����
	print_board();//���� ���带 ����ϴ� �Լ��� ȣ���Ѵ�.
}

void print_board()
{
	FILE* board;
	int arr[BOARD_SIZE][BOARD_SIZE] = { 0 };//���带 board.txt�κ��� �о�� ����Ҷ� ���Ǵ� �迭
	int arrb[BOARD_SIZE][BOARD_SIZE] = { 0 };//�� �迭�� �����ؼ� �� 3�� �̻��� �����Ҷ� �����ϴ� ���� ��� arrb�� ���� �� ����Ѵ�. 
	int i, j, n;
	int i1 = 0, j1 = 0, j2 = 0, i2 = 0;//�迭 ���� switch �Ҷ� ���Ǵ� �Լ�
	int turn = 5;//turn�� ǥ���ϴ� �Լ� turn �� 0�� �ɰ�� break �Ѵ�. 
	int tscore = 0; //������ ǥ���Ҷ� ���Ǵ� �Լ�
	int temp;
	int error = 0;//ó�� �Է��� ������ check_position�Լ��� �ҷ����� �ʱ� ���� ���� �Լ�. 

	//board.txt ���Ͽ��� ���� �о�ͼ� �迭 arr�� �ϳ��� �����Ѵ�. 
	board = fopen("board.txt", "r");
	if (board == NULL)	printf("Cannot open the file");

	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 6; j++)
		{
			fscanf(board, "%d", &n);
			arr[i][j] = n;
		}
	}

	//board ����Ѵ�.
	while (1)
	{
		//���� arr�� ������ ���� �����Ѵٸ� �����ϴ� �ڵ�
		while (match_block(arrb, arr)) {
			//1. �ٲ� ���� ���������� ǥ���� �� ���Ǵ� �ڵ�
			system("cls");
			printf("Score: %d\n", tscore);
			for (i = 0; i < 6; i++)
			{
				for (j = 0; j < 6; j++)
				{
					if (arrb[i][j]) {
						set_color_red();
						printf("%d ", arrb[i][j]);
						reset_color();
					}
					else printf("%d ", arr[i][j]);
					if (j == 5) printf("| %d\n", i);
				}
			}
			printf("------------+\n");
			printf("0 1 2 3 4 5\n");

			//2. ��� �����ϴ� ���� �������� ó���ϰ� ������ ���� random���� ���� �����ϴ� �ڵ�
			Sleep(1000);
			system("cls");
			tscore = score(arrb, tscore);
			printf("Score: %d\n", tscore);
			for (i = 0; i < 6; i++)
			{
				for (j = 0; j < 6; j++)
				{
					if (arrb[i][j]) {
						printf("  ");
						arr[i][j] = rand() % 5 + 1;
					}
					else printf("%d ", arr[i][j]);
					if (j == 5) printf("| %d\n", i);
				}
			}
			printf("------------+\n");
			printf("0 1 2 3 4 5\n");
			Sleep(1000);

			system("cls");
			printf("Score: %d\n", tscore);
			for (i = 0; i < 6; i++)
			{
				for (j = 0; j < 6; j++)
				{
					printf("%d ", arr[i][j]);
					if (j == 5) printf("| %d\n", i);
				}
			}
			printf("------------+\n");
			printf("0 1 2 3 4 5\n");
			Sleep(1000);
		}

		system("cls");
		printf("Score: %d\n", tscore);
		for (i = 0; i < 6; i++)
		{
			for (j = 0; j < 6; j++)
			{
				printf("%d ", arr[i][j]);
				if (j == 5) printf("| %d\n", i);
			}
		}
		printf("------------+\n");
		printf("0 1 2 3 4 5\n");
		printf("===============\n\n");

		if (turn < 1) break;//turn�� 0�� �Ǹ� while loop�� �����.
		if (error != 0 && check_position(i1, j1, i2, j2) == 1) printf("*** Out of range! ***\n");
		else if (error != 0 && check_position(i1, j1, i2, j2) == 2) printf("*** They are not adjacent! ***\n");
		printf("* Remaining turns: %d\n", turn); 
		printf("Swap...\n");
		printf("Block 1:");
		scanf("%d%d", &i1, &j1);
		printf("Block 2:");
		scanf("%d%d", &i2, &j2);
		error++;
		
		//���� �Է��� ��ȿ�� ��� �Է°���� �迭�� �����Ѵ�. 
		if (check_position(i1, j1, i2, j2) == 0)
		{
			temp = arr[i1][j1];
			arr[i1][j1] = arr[i2][j2];
			arr[i2][j2] = temp;
			turn--;

			//�ٲ��� ���������� ǥ��
			system("cls");
			printf("Score: %d\n", tscore);
			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					if (i == i1 && j == j1 || i == i2 && j == j2)
					{
						set_color_red();
						printf("%d ", arr[i][j]);
						reset_color();
					}
					else printf("%d ", arr[i][j]);
					if (j == 5) printf("|%2d\n", i);
				}
			}
			printf("------------+\n");
			printf("0 1 2 3 4 5\n");
			Sleep(1000);
		}
	}
	//turn�� 0�� �Ǹ� ���������� ����� ���� ������ ǥ���Ѵ�. 
	system("cls");
	printf("Score: %d\n", tscore);
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 6; j++)
		{
			printf("%d ", arr[i][j]);
			if (j == 5) printf("| %d\n", i);
		}
	}
	printf("------------+\n");
	printf("0 1 2 3 4 5\n");
	printf("===============\n\n");
	printf("** You've earned %d points! **", tscore);
}

void set_color_red() {
	printf("\033[1;31m");
}

void reset_color() {
	printf("\033[0m");
}

//�Է��� ���� ��ȿ���� �Ǵ��ϴ� �Լ�
int check_position(int i1, int j1, int i2, int j2)
{
	//��: ������ ��� �Է��� ���
	if (i1 < 0 || j1 < 0 || i2 < 0 || j2 < 0 || i1>5 || j1>5 || i2>5 || j2 >5) return 1;
	//��: ������ �� ���� �ƴ� ���
	//���� ��� 2 return
	else if ((i1 == i2) && (j1 == j2)) return 2;
	//���� �ƴѰ�� 2 return
	else if (!((abs(i1 - i2) < 2&&(j1==j2)) || (abs(j1 - j2) < 2 && (i1 == i2)))) return 2;
	else return 0;
}

//���� �����ؼ� ���ϴ� �Լ�
int score(int arrb[][BOARD_SIZE], int total_score) {
	int sum = 0;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			sum += arrb[i][j];
		}
	}
	return sum + total_score;
}

//�����ϴ� ���� 3������ �Ǵ��ϴ� �Լ�
int match_block(int arrb[][BOARD_SIZE], int arr[][BOARD_SIZE])
{
	int i, j;

	//arrb ���ҵ� 0���� �ʱ�ȭ
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 6; j++)
		{
			arrb[i][j] = 0;
		}
	}

	//���� ���� 3�� �̻��� ��ġ�Ѵٸ�, arr���Ҹ� arrb�� �ִ´�. 
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 6; j++)
		{
			if (arr[i][j] == arr[i + 1][j] && arr[i + 1][j] == arr[i + 2][j])
			{
				arrb[i][j] = arr[i][j];
				arrb[i + 1][j] = arr[i+1][j];
				arrb[i + 2][j] = arr[i+2][j];
			}
		}
	}

	//���� ���� 3�� �̻��� ��ġ�Ѵٸ�, arr���Ҹ� arrb�� �ִ´�. 
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (arr[i][j] == arr[i][j + 1] && arr[i][j + 1] == arr[i][j + 2])
			{
				arrb[i][j] = arr[i][j];
				arrb[i][j + 1] = arr[i][j + 1];
				arrb[i][j + 2] = arr[i][j + 2];
			}
		}
	}

	//�迭�˻��� 0�� �ƴ� ���� �ϳ��� ����ִٸ� 1��ȯ
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 6; j++)
		{
			if (arrb[i][j]) return 1;
		}
	}
	return 0;
}

