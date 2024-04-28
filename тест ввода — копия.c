#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

#define USED 1
#define UNUSED 0
#define TRUE 1
#define FALSE 0

/*сломаны:*/
#define DEBUG_MODE 0
#define DEBUG_MODE_ALL 0

/*структура заготовки*/
struct Board
{
        int length;                         /*длина заготовки*/
        int counter;                        /*количество деталей на заготовке*/
        int best_counter;           /*минимальное временное колличество деталей на заготовке*/
        int remnat;                         /*длина обрезка*/
        bool used;                          /*найдена ли комбинация для этой заготовки?*/
        int* combination;           /*указатель на массив, в котором будут перебираться комбинации*/
        int* buffer;                        /*указатель на массив, в котором будут храниться временные лучшие комбинации*/
        int* best_combination;  /*указатель на массив, в котором будет храниться итоговая комбинация*/
};

/*структура детали*/
struct Part
{
        int length; /*длина детали*/
        bool used;  /*использована ли она в итоговой комбинации?*/
};

struct record
{
        int length;
        int count;
        bool valid;
};

int length_parts = 1;
int length_boards = 0;

struct Part* part = NULL;
struct Board* board = NULL;

bool leter(int c)
{
        return (!(c >= '0' && c <= '9') && !(c == ' ' || c == '\t'));
}

bool number(int c)
{
        return (c >= '0' && c <= '9');
}

bool space(int c)
{
        return (c == ' ' || c == '\t');
}

/*(не)безопасный ввод целых чисел*/
void input(FILE* fp)
{
        #define M_LENGTH 1
        #define M_COUNT 2
        #define M_SKIP 3

        #define SM_IN 1
        #define SM_OUT 2
        #define SM_LETER 3

        char simbol = 0;
        bool valid = TRUE;
        int length = 0;
        int count = 0;
        int mode = M_LENGTH;
        int sub_mode = SM_OUT;

        while((simbol = fgetc(fp)) != EOF)
        {
                switch (mode)
                {
                        case(M_LENGTH):
                        {
                                valid = TRUE;
                                if(space(simbol))
                                {
                                        if(sub_mode == SM_OUT)
                                        {
                                                //break;
                                        }
                                        else if(sub_mode == SM_IN)
                                        {
                                                mode = M_COUNT;
                                                sub_mode = SM_OUT;
                                        }

                                }
                                else if(number(simbol))
                                {
                                        if(sub_mode == SM_OUT)
                                        {
                                                length = 0;
                                                sub_mode = SM_IN;
                                        }
                                        else if(sub_mode == SM_IN)
                                        {
                                                //;
                                        }
                                        length *= 10;
                                        length += simbol - '0';
                                }
                                else if(leter(simbol))
                                {
                                        if(simbol == '\n')
                                        {
                                                mode = M_LENGTH;
                                        }
                                        else
                                        {
                                                mode = M_SKIP;
                                        }
                                        valid = FALSE;
                                }
                        }
                        break;

                        case(M_COUNT):
                        {
                                if(space(simbol))
                                {
                                        if(sub_mode == SM_OUT)
                                        {
                                                //break;
                                        }
                                        else if(sub_mode == SM_IN)
                                        {
                                                mode = M_SKIP;
                                                sub_mode = SM_OUT;
                                        }
                                }
                                else if(number(simbol))
                                {
                                        if(sub_mode == SM_OUT)
                                        {
                                                count = 0;
                                                sub_mode = SM_IN;
                                        }
                                        else if(sub_mode == SM_IN)
                                        {
                                                //;
                                        }
                                        count *= 10;
                                        count += simbol - '0';
                                }
                                else if(leter(simbol))
                                {
                                        if(simbol == '\n')
                                        {
                                                mode = M_LENGTH;
                                        }
                                        else
                                        {
                                                mode = M_SKIP;
                                        }
                                        valid = FALSE;
                                }
                        }
                        break;

                        case(M_SKIP):
                        {
                                if(simbol == '\n')
                                {
                                        mode = M_LENGTH;
                                }
                        }
                        break;
                }
        }


        /**
        *сделаны:
        *счет полной, безошибочной строки
        *счет обрывающейся на количестве
        */
}


/*сосчитать колличество цифр в цисле (для красивого отступа)*/
int digit_count(int number)
{
        int i;
        for(i=0; number != 0; i++)
                number /= 10;

        return i;
}


/*распечатать итоговую комбинацию для всех заготовок*/
void print_combin()
{
        printf("Введенные детали:\n");
        for(int i=0; i<length_parts; i++)
        {
                printf("%d\t%d\t%d\n", i+1, part[i].length, part[i].used);
        }

        printf("\nЗаготовки:\n");

        for(int i=0; i<length_boards; i++)
        {
                printf("№%d\t%d\t%d\n", i+1, board[i].length, board[i].used);
        }
        printf("\n\n");
}


int main()
{
        /*установить кодировку utf-8 в консоли для вывода кирилицы*/
        SetConsoleOutputCP(CP_UTF8);


        FILE* finput = fopen("input.txt", "r");
        input(finput);

        print_combin();

        fclose(finput);

        system("pause");
        return 0;
}

