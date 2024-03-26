#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BOARD_SIZE 6
#include <windows.h>

void print_board();//보드 화면을 출력하는 함수 이곳에서 색도 바꾸고 공백 처리한다. 
int check_position(int i1, int j1, int i2, int j2);//입력받은 위치가 유효한지, 옆에 위치하는지 체크한다. 
void set_color_red();//색을 빨강으로 바꾸기 위한 함수
void reset_color();//색을 빨강으로 바꾸기 위한 함수
int score(int arrb[][BOARD_SIZE], int total_score);//점수를 누적해서 더하는 함수
int match_block(int arrb[][BOARD_SIZE],int arr[][BOARD_SIZE]);//연속해서 3개 이상의 수가 일치할 경우 처리하는 함수

int main()
{//게임시작화면 출력
	char enter;
	srand(time(NULL));
	printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("CSED 101\n");
	printf("Assignment 2\n\n");
	printf("[[  Match-3 Game   ]]\n\n");
	printf("Press [Enter] to start\n");
	printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	scanf("%c", &enter);//enter외의 입력은 고려하지 않으므로 처리하지 않음
	print_board();//게임 보드를 출려하는 함수를 호출한다.
}

void print_board()
{
	FILE* board;
	int arr[BOARD_SIZE][BOARD_SIZE] = { 0 };//보드를 board.txt로부터 읽어와 출려할때 사용되는 배열
	int arrb[BOARD_SIZE][BOARD_SIZE] = { 0 };//이 배열은 연속해서 수 3개 이상이 연속할때 연속하는 수를 모두 arrb에 넣을 때 사용한다. 
	int i, j, n;
	int i1 = 0, j1 = 0, j2 = 0, i2 = 0;//배열 값을 switch 할때 사용되는 함수
	int turn = 5;//turn을 표시하는 함수 turn 이 0이 될경우 break 한다. 
	int tscore = 0; //점수를 표시할때 사용되는 함수
	int temp;
	int error = 0;//처음 입력을 받을때 check_position함수를 불러오지 않기 위해 만든 함수. 

	//board.txt 파일에서 수를 읽어와서 배열 arr에 하나씩 저정한다. 
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

	//board 출력한다.
	while (1)
	{
		//만약 arr중 세개의 수가 연속한다면 실행하는 코드
		while (match_block(arrb, arr)) {
			//1. 바꾼 수를 빨간색으로 표현할 때 사용되는 코드
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

			//2. 모든 연속하는 수를 공백으로 처리하고 지워진 수에 random으로 수를 배정하는 코드
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

		if (turn < 1) break;//turn이 0이 되면 while loop을 벗어난다.
		if (error != 0 && check_position(i1, j1, i2, j2) == 1) printf("*** Out of range! ***\n");
		else if (error != 0 && check_position(i1, j1, i2, j2) == 2) printf("*** They are not adjacent! ***\n");
		printf("* Remaining turns: %d\n", turn); 
		printf("Swap...\n");
		printf("Block 1:");
		scanf("%d%d", &i1, &j1);
		printf("Block 2:");
		scanf("%d%d", &i2, &j2);
		error++;
		
		//만약 입력이 유효할 경우 입력값대로 배열을 변경한다. 
		if (check_position(i1, j1, i2, j2) == 0)
		{
			temp = arr[i1][j1];
			arr[i1][j1] = arr[i2][j2];
			arr[i2][j2] = temp;
			turn--;

			//바꾼후 빨간색으로 표시
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
	//turn이 0이 되면 마지막으로 보드와 최종 점수를 표시한다. 
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

//입력한 값이 유효한지 판단하는 함수
int check_position(int i1, int j1, int i2, int j2)
{
	//가: 범위를 벗어난 입력일 경우
	if (i1 < 0 || j1 < 0 || i2 < 0 || j2 < 0 || i1>5 || j1>5 || i2>5 || j2 >5) return 1;
	//나: 인접한 두 블럭이 아닌 경우
	//같은 경우 2 return
	else if ((i1 == i2) && (j1 == j2)) return 2;
	//옆이 아닌경우 2 return
	else if (!((abs(i1 - i2) < 2&&(j1==j2)) || (abs(j1 - j2) < 2 && (i1 == i2)))) return 2;
	else return 0;
}

//점수 누적해서 더하는 함수
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

//연속하는 수가 3개인지 판단하는 함수
int match_block(int arrb[][BOARD_SIZE], int arr[][BOARD_SIZE])
{
	int i, j;

	//arrb 원소들 0으로 초기화
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 6; j++)
		{
			arrb[i][j] = 0;
		}
	}

	//만약 세로 3개 이상이 일치한다면, arr원소를 arrb에 넣는다. 
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

	//만약 가로 3개 이상이 일치한다면, arr원소를 arrb에 넣는다. 
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

	//배열검사후 0이 아닌 수가 하나라도 들어있다면 1반환
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 6; j++)
		{
			if (arrb[i][j]) return 1;
		}
	}
	return 0;
}

