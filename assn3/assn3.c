#define _CRT_SECURE_NO_WARNINGS
#define MAX_FILE_NAME 30
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

void training(char* filename);//train�Ҷ� ���Ǵ� �Լ�
void test();//test �Ҷ� ���Ǵ� �Լ�
int findword(char** words, char** hswords, int n, int a);//�ܾ ��ġ�� Ƚ���� ���� �Լ�
int compword(char* ptr, char** words, int num, int start);//�ռ� ����� �ܾ�� �������� �ʰ� �ϱ� ���� ���� ����
void swap(char** a, char** b);//���ĺ� ������� ������ ���� ���� �Լ�

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
        //�ҹ��� ��ȯ
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
    char message[1000];//ó�� ���� ���� �����ϴ� �迭
    char pmessage[1000];//�������� ���ڿ� ����Ʈ ���� ó�� ���� ���� �����ϴ� �迭
    char* p;
    char** words;//�׽�Ʈ �ܾ� �߶� �����ϴ� �迭
    int wnum = 0;//�ܾ� �� ���� ����
    int a = 5, b = 5;
    char** stwords;//stat ���� �ܾ� �߶� �����ϴ� �迭
    int* hfreq;//stat���� ham�� �� ����
    int* sfreq;//stat���� spam �� ����
    double* hprob;//P(wi | Ham) ���� 
    double* sprob;//P(wi | Spam) ���� 
    int hnum;
    int snum;
    int hstot;
    int stnum = 0;
    double hfinal, sfinal;
    char* ptr;
    char input;

    char str[1000];
    //stat ���� �ҷ�����
    FILE* fp = fopen("stats.txt", "r");
    stwords = (char**)malloc(b * sizeof(char*));
    hfreq = (int*)malloc(b * sizeof(int));
    sfreq = (int*)malloc(b * sizeof(int));

    //�ѹ��徿 �о Ham/Spam �� ����
    fgets(str, 1000, fp);
    ptr = strtok(str, ":, \n");
    ptr = strtok(NULL, ":, \n");
    hnum = atoi(ptr);
    ptr = strtok(NULL, ":, \n");
    ptr = strtok(NULL, ":, \n");
    snum = atoi(ptr);
    hstot = hnum + snum;

    //���� ���������� �б� �ݺ�
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

        //�ܾ 5 ������ �ٽ� ���Ҵ�
        if (stnum > b - 1)
        {
            b = b * 2;
            stwords = (char**)realloc(stwords, b * sizeof(char*));
            hfreq = (int*)realloc(hfreq, b * sizeof(int));
            sfreq = (int*)realloc(sfreq, b * sizeof(int));
        }
    }
    fclose(fp);

    //�׽�Ʈ �ϰ� ���� ���� �Է��ϱ�
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
    
    //���� �Է��� ���ڿ��� ��ȣ�ϰ�� ���� ó��
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
        //���� ���忡�� �ܾ� �ݺ��Ǹ� ����x
        if (compword(ptr, words, wnum, 0))
        {
            ptr = strtok(NULL, " \t\n");
            continue;
        }
        //�ܾ stwords�� ������ ���� ����
        if (!(compword(ptr, stwords, stnum, 0)))
        {
            ptr = strtok(NULL, " \t\n");
            continue;
        }
        words[wnum] = (char*)malloc((strlen(ptr) + 1) * sizeof(char));
        strcpy(words[wnum], ptr);
        ptr = strtok(NULL, " \t\n");
        wnum++;

        //���Ҵ�
        if (wnum > a - 1)
        {
            a = a * 2;
            words = (char**)realloc(words, a * sizeof(char*));
            hprob = (double*)realloc(hprob, a * sizeof(double));
            sprob = (double*)realloc(sprob, a * sizeof(double));
        }
    }//�Է��� �޽��� words�� ���� ��

    //P(wi | Spam) ����ؼ� �����ϱ�
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

    //P(Spam) ���
    hfinal = (double)hnum / hstot;
    sfinal = (double)snum / hstot;

    printf("\nP(Ham) = %.3lf", hfinal);
    printf(", P(Spam) = %.3lf\n\n", sfinal);

    for (int i = 0; i < wnum; i++)
    {
        printf("P(%s | Ham) = %.3lf", words[i], hprob[i]);
        printf(", P(%s | Spam) = %.3lf\n", words[i], sprob[i]);
    }

    //���� Ham/Spam ���
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

    //���� �Ҵ� ����
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

    //���� �Է¹����� �޴��� ���ư���
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
    //�Է¹��� ���� ����
    FILE* fp = fopen(filename, "r");
    char** hwords;//Ham ������ �ܾ� ����
    char** swords;//Spam ������ �ܾ� ����
    char** words;//txt ������ ��� ���� �ܾ� ����
    int hnum = 0;//hwords �迭�� ����� ���� ���� ����
    int snum = 0;//swords �迭�� ����� ���� ���� ����
    int wnum = 0;//words �迭�� ����� ���� ���� ����

    char str[1000];//ó�� ������ �����ϴ� ����
    int* hfreq;//ham���忡�� �ܾ ���� Ƚ���� ���� ����
    int* sfreq;//ham���忡�� �ܾ ���� Ƚ���� ���� ����

    char* p;
    int a = 5, b = 5, c = 5;
    char input;
    int start = 0;//���� ���忡�� ������ �����ߴ� �Ȱ��� �ܾ� ó���ϴ� ����

    //�޸� �����Ҵ�
    words = (char**)malloc(c * sizeof(char*));
    hfreq = (int*)malloc(c * sizeof(int));
    sfreq = (int*)malloc(c * sizeof(int));
    
    //���� ���� ��ü�� �о��
    while (fgets(str, 1000, fp) != NULL)
    {
        _strlwr(str);
        p = str;

        //���ĺ� �ƴҰ�� ����� �ٲٱ�
        while (*p != '\0')
        {
            if (*p < 97 || *p>122) *p = ' ';
            p++;
        }

        //�ܾ� �ڸ���
        char* ptr = strtok(str, " \t\n");
        while (ptr != NULL) {
            //���� �� 1�� 20�� ó��
            if (strlen(ptr) <= 1 || strlen(ptr) > 20)
            {
                ptr = strtok(NULL, " \t\n");
                continue;
            }
            //���Դ� ���� �� ������ ��� ó��
            if (compword(ptr, words, wnum, 0))
            {
                ptr = strtok(NULL, " \t\n");
                continue;
            }
            words[wnum] = (char*)malloc((strlen(ptr) + 1) * sizeof(char));
            strcpy(words[wnum], ptr);
            ptr = strtok(NULL, " \t\n");
            wnum++;
            //5������ ���Ҵ�
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

    //���ĺ� ������ ����
    for (int i = 0; i < wnum - 1; i++)
    {
        for (int j = 0; j < wnum - i - 1; j++)
        {
            if (strcmp(words[j], words[j + 1]) > 0)
                swap(&words[j], &words[j + 1]);
        }
    }

    //�ٽ� ���� ��� ham/spam������ ���� ���ε��� ����
    fopen(filename, "r");
    hwords = (char**)malloc(a * sizeof(char*));//�ۿ��� �Ҵ��ϴ� ��, �� �ܾ��� �� ����
    swords = (char**)malloc(b * sizeof(char*));

    while (fgets(str, 1000, fp) != NULL)//���� ��ü�� �о��
    {
        _strlwr(str);
        p = str;
        while (*p != '\0')
        {
            if (*p < 97 || *p>122) *p = ' ';
            p++;
        }

        char* ptr = strtok(str, " \t\n");

        //ù �ܾ ham�ΰ��
        if (strcmp(ptr, "ham") == 0)
        {
            while (ptr != NULL) {

                //1�� 20 ó��
                if (strlen(ptr) <= 1 || strlen(ptr) > 20)
                {
                    ptr = strtok(NULL, " \t\n");
                    continue;
                }
                //���� ���� ���忡�� �ܾ �ι� ���ð�� �迭�� �������� ����
                if (compword(ptr, hwords, hnum, start))
                {
                    ptr = strtok(NULL, " \t\n");
                    continue;
                }
                hwords[hnum] = (char*)malloc((strlen(ptr) + 1) * sizeof(char));
                strcpy(hwords[hnum], ptr);
                ptr = strtok(NULL, " \t\n");
                hnum++;

                //���Ҵ�
                if (hnum > a - 1)
                {
                    a = a * 2;
                    hwords = (char**)realloc(hwords, a * sizeof(char*));
                }
            }
            start = hnum;
        }

        //ù �ܾ spam�ΰ��
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

    //�ܾ� Ƚ�� ����
    for (int i = 0; i < wnum; i++)
    {
        hfreq[i] = findword(words, hwords, i, hnum);
    }

    //�ܾ� Ƚ�� ����
    for (int i = 0; i < wnum; i++)
    {
        sfreq[i] = findword(words, swords, i, snum);
    }

    printf("\nAfter training...\n");

    //Ham/Spam �� ����Ʈ
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

    //Ham/Spam �� ���Ͽ� ����Ʈ
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
    
    //�Ҵ� ����
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