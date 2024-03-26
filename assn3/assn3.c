#define _CRT_SECURE_NO_WARNINGS
#define MAX_FILE_NAME 30
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

void training(char* filename);//train할때 사용되는 함수
void test();//test 할때 사용되는 함수
int findword(char** words, char** hswords, int n, int a);//단어가 겹치는 횟수를 세는 함수
int compword(char* ptr, char** words, int num, int start);//앞서 저장된 단어는 저장하지 않게 하기 위해 만든 변수
void swap(char** a, char** b);//알파벳 순서대로 정렬을 위해 만든 함수

int main()
{
    while (1) {
        char command[10];
        char letter[10];
        char filename[MAX_FILE_NAME];
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
        printf("CSED 101\n");
        printf("Assignment 3\n\n");
        printf("Naive Bayesian Classifier for Spam Filtering\n");
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
        printf("Command: ");
        scanf("%s", command);
        //소문자 전환
        strcpy(letter, _strlwr(command));

        //train
        if (!(strcmp(letter, "train")))
        {
            printf("File name: ");
            scanf("%s", filename);
            FILE* fp = fopen(filename, "r");
            if (fp == NULL)
            {
                printf("Error: File does not exist");
                Sleep(1000);
                system("cls");
                continue;

            }
            fclose(fp);
            training(filename);
        }

        //test
        else if (!(strcmp(letter, "test")))
        {
            FILE* fp = fopen("stats.txt", "r");
            if (fp == NULL)
            {
                printf("Error: File does not exist");
                Sleep(1000);
                system("cls");
                continue;
            }
            fclose(fp);
            test();
        }

        //exit
        else if (!(strcmp(letter, "exit")))
        {
            break;
        }

        //error
        else
        {
            printf("Error: Invalid input");
            Sleep(1000);
            system("cls");
            continue;
        }
    }
    return 0;
}

void test()
{
    char message[1000];//처음 읽은 문장 저장하는 배열
    char pmessage[1000];//마지막에 문자열 프린트 위해 처음 읽은 문장 복사하는 배열
    char* p;
    char** words;//테스트 단어 잘라서 저장하는 배열
    int wnum = 0;//단어 수 세는 변수
    int a = 5, b = 5;
    char** stwords;//stat 에서 단어 잘라서 저장하는 배열
    int* hfreq;//stat에서 ham의 수 저장
    int* sfreq;//stat에서 spam 수 저장
    double* hprob;//P(wi | Ham) 저장 
    double* sprob;//P(wi | Spam) 저장 
    int hnum;
    int snum;
    int hstot;
    int stnum = 0;
    double hfinal, sfinal;
    char* ptr;
    char input;

    char str[1000];
    //stat 파일 불러오기
    FILE* fp = fopen("stats.txt", "r");
    stwords = (char**)malloc(b * sizeof(char*));
    hfreq = (int*)malloc(b * sizeof(int));
    sfreq = (int*)malloc(b * sizeof(int));

    //한문장씩 읽어서 Ham/Spam 수 저장
    fgets(str, 1000, fp);
    ptr = strtok(str, ":, \n");
    ptr = strtok(NULL, ":, \n");
    hnum = atoi(ptr);
    ptr = strtok(NULL, ":, \n");
    ptr = strtok(NULL, ":, \n");
    snum = atoi(ptr);
    hstot = hnum + snum;

    //문장 끝날때까지 읽기 반복
    while (fgets(str, 1000, fp) != NULL)
    {
        ptr = strtok(str, ",\n");
        stwords[stnum] = (char*)malloc((strlen(ptr) + 1) * sizeof(char));
        strcpy(stwords[stnum], ptr);
        ptr = strtok(NULL, ", \n");
        hfreq[stnum] = atoi(ptr);
        ptr = strtok(NULL, ", \n");
        sfreq[stnum] = atoi(ptr);
        ptr = strtok(NULL, ":, \n");
        stnum++;

        //단어가 5 넘으면 다시 재할당
        if (stnum > b - 1)
        {
            b = b * 2;
            stwords = (char**)realloc(stwords, b * sizeof(char*));
            hfreq = (int*)realloc(hfreq, b * sizeof(int));
            sfreq = (int*)realloc(sfreq, b * sizeof(int));
        }
    }
    fclose(fp);

    //테스트 하고 싶은 문장 입력하기
    words = (char**)malloc(a * sizeof(char*));
    hprob = (double*)malloc(a * sizeof(double));
    sprob = (double*)malloc(a * sizeof(double));
    printf("Enter a message: ");
    fseek(stdin, 0, SEEK_END);
    
    //scan
    fgets(message, 1000, stdin);
    ptr = strtok(message, "\n");
    strcpy(pmessage, message);
    _strlwr(message);
    p = message;
    
    //만약 입력한 문자열이 기호일경우 띄어쓰기 처리
    while (*p != '\0')
    {
        if (*p < 97 || *p>122) *p = ' ';
        p++;
    }
    ptr = strtok(message, " \t\n");

    while (ptr != NULL) {
        if (strlen(ptr) <= 1 || strlen(ptr) > 20)
        {
            ptr = strtok(NULL, " \t\n");
            continue;
        }
        //같은 문장에서 단어 반복되면 저장x
        if (compword(ptr, words, wnum, 0))
        {
            ptr = strtok(NULL, " \t\n");
            continue;
        }
        //단어가 stwords에 없으면 저장 안함
        if (!(compword(ptr, stwords, stnum, 0)))
        {
            ptr = strtok(NULL, " \t\n");
            continue;
        }
        words[wnum] = (char*)malloc((strlen(ptr) + 1) * sizeof(char));
        strcpy(words[wnum], ptr);
        ptr = strtok(NULL, " \t\n");
        wnum++;

        //재할당
        if (wnum > a - 1)
        {
            a = a * 2;
            words = (char**)realloc(words, a * sizeof(char*));
            hprob = (double*)realloc(hprob, a * sizeof(double));
            sprob = (double*)realloc(sprob, a * sizeof(double));
        }
    }//입력한 메시지 words에 저장 끝

    //P(wi | Spam) 계산해서 저장하기
    for (int i = 0; i < wnum; i++)
    {
        for (int j = 0; j < stnum; j++)
        {
            if (strcmp(words[i], stwords[j]) == 0)
            {
                hprob[i] = (hfreq[j] / (double)hnum);
                if (hprob[i] == 0)  hprob[i] = 0.1 / hnum;
                sprob[i] = (sfreq[j] / (double)snum);
                if (sprob[i] == 0)  sprob[i] = 0.1 / snum;
            }
        }
    }

    //P(Spam) 계산
    hfinal = (double)hnum / hstot;
    sfinal = (double)snum / hstot;

    printf("\nP(Ham) = %.3lf", hfinal);
    printf(", P(Spam) = %.3lf\n\n", sfinal);

    for (int i = 0; i < wnum; i++)
    {
        printf("P(%s | Ham) = %.3lf", words[i], hprob[i]);
        printf(", P(%s | Spam) = %.3lf\n", words[i], sprob[i]);
    }

    //최종 Ham/Spam 계산
    for (int i = 0; i < wnum; i++)
    {
        hfinal *= hprob[i];
        sfinal *= sprob[i];
    }
    printf("\nP( Ham | '%s') : %.3lf\n", pmessage, hfinal);
    printf("P(Spam | '%s') : %.3lf\n\n", pmessage, sfinal);

    if (hfinal > sfinal)
        printf("This message is HAM");
    else if (hfinal < sfinal)
        printf("This message is SPAM");
    else
        printf("Spam == Ham");

    //동적 할당 해제
    for (int i = 0; i < wnum; i++)
    {
        free(words[i]);
    }
    for (int i = 0; i < stnum; i++)
    {
        free(stwords[i]);
    }
    free(words);
    free(stwords);
    free(hfreq);
    free(sfreq);
    free(hprob);
    free(sprob);

    //엔터 입력받으면 메뉴로 돌아가기
    while (1) {
        scanf("%c", &input);
        if (input == '\n')
        {
            system("cls");
            break;
        }
        else
            printf("please press 'enter'!");
    }

    return;
}


void training(char* filename)
{
    //입력받은 파일 열기
    FILE* fp = fopen(filename, "r");
    char** hwords;//Ham 문장의 단어 저장
    char** swords;//Spam 문장의 단어 저장
    char** words;//txt 파일의 모든 문장 단어 저장
    int hnum = 0;//hwords 배열에 저장된 원소 세는 변수
    int snum = 0;//swords 배열에 저장된 원소 세는 변수
    int wnum = 0;//words 배열에 저장된 원소 세는 변수

    char str[1000];//처음 문장을 저장하는 변수
    int* hfreq;//ham문장에서 단어가 사용된 횟수를 세는 변수
    int* sfreq;//ham문장에서 단어가 사용된 횟수를 세는 변수

    char* p;
    int a = 5, b = 5, c = 5;
    char input;
    int start = 0;//같은 문장에서 이전에 저장했던 똑같은 단어 처리하는 변수

    //메모리 동적할당
    words = (char**)malloc(c * sizeof(char*));
    hfreq = (int*)malloc(c * sizeof(int));
    sfreq = (int*)malloc(c * sizeof(int));
    
    //문장 한줄 전체를 읽어옴
    while (fgets(str, 1000, fp) != NULL)
    {
        _strlwr(str);
        p = str;

        //알파벳 아닐경우 띄어쓰기로 바꾸기
        while (*p != '\0')
        {
            if (*p < 97 || *p>122) *p = ' ';
            p++;
        }

        //단어 자르기
        char* ptr = strtok(str, " \t\n");
        while (ptr != NULL) {
            //글자 수 1개 20개 처리
            if (strlen(ptr) <= 1 || strlen(ptr) > 20)
            {
                ptr = strtok(NULL, " \t\n");
                continue;
            }
            //나왔던 문자 또 나왔을 경우 처리
            if (compword(ptr, words, wnum, 0))
            {
                ptr = strtok(NULL, " \t\n");
                continue;
            }
            words[wnum] = (char*)malloc((strlen(ptr) + 1) * sizeof(char));
            strcpy(words[wnum], ptr);
            ptr = strtok(NULL, " \t\n");
            wnum++;
            //5넘으면 재할당
            if (wnum > c - 1)
            {
                c = c * 2;
                words = (char**)realloc(words, c * sizeof(char*));
                hfreq = (int*)realloc(hfreq, c * sizeof(int));
                sfreq = (int*)realloc(sfreq, c * sizeof(int));
            }
        }
    }
    fclose(fp);

    //알파벳 순서로 정리
    for (int i = 0; i < wnum - 1; i++)
    {
        for (int j = 0; j < wnum - i - 1; j++)
        {
            if (strcmp(words[j], words[j + 1]) > 0)
                swap(&words[j], &words[j + 1]);
        }
    }

    //다시 파일 열어서 ham/spam에서의 문장 따로따로 저장
    fopen(filename, "r");
    hwords = (char**)malloc(a * sizeof(char*));//밖에서 할당하는 것, 즉 단어의 총 개수
    swords = (char**)malloc(b * sizeof(char*));

    while (fgets(str, 1000, fp) != NULL)//한줄 전체를 읽어옴
    {
        _strlwr(str);
        p = str;
        while (*p != '\0')
        {
            if (*p < 97 || *p>122) *p = ' ';
            p++;
        }

        char* ptr = strtok(str, " \t\n");

        //첫 단어가 ham인경우
        if (strcmp(ptr, "ham") == 0)
        {
            while (ptr != NULL) {

                //1과 20 처리
                if (strlen(ptr) <= 1 || strlen(ptr) > 20)
                {
                    ptr = strtok(NULL, " \t\n");
                    continue;
                }
                //만약 같은 문장에서 단어가 두번 나올경우 배열에 저장하지 않음
                if (compword(ptr, hwords, hnum, start))
                {
                    ptr = strtok(NULL, " \t\n");
                    continue;
                }
                hwords[hnum] = (char*)malloc((strlen(ptr) + 1) * sizeof(char));
                strcpy(hwords[hnum], ptr);
                ptr = strtok(NULL, " \t\n");
                hnum++;

                //재할당
                if (hnum > a - 1)
                {
                    a = a * 2;
                    hwords = (char**)realloc(hwords, a * sizeof(char*));
                }
            }
            start = hnum;
        }

        //첫 단어가 spam인경우
        else if (strcmp(ptr, "spam") == 0)
        {
            while (ptr != NULL) {
                if (strlen(ptr) <= 1 || strlen(ptr) > 20)
                {
                    ptr = strtok(NULL, " \t\n");
                    continue;
                }
                if (compword(ptr, swords, snum, start))
                {
                    ptr = strtok(NULL, " \t\n");
                    continue;
                }
                swords[snum] = (char*)malloc((strlen(ptr) + 1) * sizeof(char));
                strcpy(swords[snum], ptr);
                ptr = strtok(NULL, " \t\n");
                snum++;
                if (snum > b - 1)
                {
                    b = b * 2;
                    swords = (char**)realloc(swords, b * sizeof(char*));
                }
            }
            start = snum;
        }
    }
    fclose(fp);

    //단어 횟수 세기
    for (int i = 0; i < wnum; i++)
    {
        hfreq[i] = findword(words, hwords, i, hnum);
    }

    //단어 횟수 세기
    for (int i = 0; i < wnum; i++)
    {
        sfreq[i] = findword(words, swords, i, snum);
    }

    printf("\nAfter training...\n");

    //Ham/Spam 수 프린트
    for (int i = 0; i < wnum; i++) {
        if (strcmp(words[i], "ham") == 0)
            printf("Ham:%d", hfreq[i]);
        if (strcmp(words[i], "spam") == 0)
            printf(", Spam:%d", sfreq[i]);
    }
    printf("\n");
    for (int i = 0; i < wnum; i++)
    {
        if ((strcmp(words[i], "ham") != 0) && (strcmp(words[i], "spam") != 0))
            printf("%s,%d,%d\n", words[i], hfreq[i], sfreq[i]);
    }

    //Ham/Spam 수 파일에 프린트
    FILE* of = fopen("stats.txt", "w");
    for (int i = 0; i < wnum; i++) {
        if (strcmp(words[i], "ham") == 0)
            fprintf(of, "Ham:%d", hfreq[i]);
        if (strcmp(words[i], "spam") == 0)
            fprintf(of, ", Spam:%d", sfreq[i]);
    }
    fprintf(of, "\n");
    for (int i = 0; i < wnum; i++)
    {
        if ((strcmp(words[i], "ham") != 0) && (strcmp(words[i], "spam") != 0))
            fprintf(of, "%s,%d,%d\n", words[i], hfreq[i], sfreq[i]);
    }
    fclose(of);
    
    //할당 해제
    for (int i = 0; i < wnum; i++)
    {
        free(words[i]);
    }

    for (int i = 0; i < hnum; i++)
    {
        free(hwords[i]);
    }

    for (int i = 0; i < snum; i++)
    {
        free(swords[i]);
    }

    free(words);
    free(hwords);
    free(swords);
    free(hfreq);
    free(sfreq);

    fseek(stdin, 0, SEEK_END);
    while (1) {
        scanf("%c", &input);
        if (input == '\n')
        {
            system("cls");
            break;
        }
        else
            printf("please press 'enter'!");
    }
    return;
}

int compword(char* ptr, char** words, int num, int start)
{
    for (int i = start; i < num; i++)
    {
        if (strcmp(ptr, words[i]) == 0) return 1;
    }
    return 0;
}

int findword(char** words, char** hswords, int n, int a)
{
    int count = 0;
    for (int i = 0; i < a; i++)
    {
        if (strcmp(words[n], hswords[i]) == 0)
            count++;
    }
    return count;
}

void swap(char** a, char** b)
{
    char** temp = *a;
    *a = *b;
    *b = temp;
}