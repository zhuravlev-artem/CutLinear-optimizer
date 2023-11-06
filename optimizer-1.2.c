#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <locale.h>

#define lenght_parts 11
#define lenght_boards 6
#define EBUG_MODE 0
#define SEND_MODE 0

/*

*/

struct
{
    int length;
    bool used;
} part[11]= {
    {0, 1},
    {1, 1},
    {4, 1},
    {7, 1},
    {2, 1},
    {4, 1},
    {8, 1},
    {1, 1},
    {10, 1},
    {5, 1},
    {6, 1},
};

struct
{
    int length;
    int part_counter;
    int old_remnat;
    bool used;
    int combination[11];
    int parts_buffer[11];
    int best_combination[11];
} board[6]={
    {10, 0, 10000, 1, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {2, 0, 10000, 1, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {4, 0, 10000, 1, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {16, 0, 10000, 1, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {8, 0, 10000, 1, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {6, 0, 10000, 1, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
};

int disk_thickness=3;



void dprint()
{

    printf("основная раскладка:\n");
    for(int i=0; i<6; i++)
    {
        printf("(%d):\t", board[i].length);
        for(int j=0; j<11; j++)
        {
            printf("%d;\t", part[board[i].combination[j]].length);
        }
        printf("обрезок: %d\n", board[i].old_remnat);
    }
    printf("\n");


    printf("буфер:\n");
    for(int i=0; i<6; i++)
    {
        printf("(%d):\t", board[i].length);
        for(int j=0; j<11; j++)
        {
            printf("%d;\t", part[board[i].parts_buffer[j]].length);
        }
        printf("\n");
    }
    printf("\n");


    printf("итог:\n");
    for(int i=0; i<6; i++)
    {
        printf("(%d):\t", board[i].length);

        for(int j=0; j<11; j++)
        {
            printf("%d;\t", part[board[i].best_combination[j]].length);
        }
        printf("\n");
    }
    printf("\n");

    printf("используются:\t");
    for(int i=1; i<11; i++)
    {
        printf("%d:%d; ", part[i].length, part[i].used);
    }
    printf("\n");

    system("pause");
}


/*
пробежаться по всем позициям, сложить длины деталей под индексами позиции
с учётом полщины диска
*/
int summ_length_parts(int b)
{
    int buff=0;
    for(int i=0; i < lenght_parts; i++)
    {

        buff += part[board[b].combination[i]].length;

        if(i > 0 && board[b].combination[i] != 0)
        {
            buff += disk_thickness;
        }
    }
    return buff;
}

/*
если длинна всех деталей на доске вместе с новой деталью
не больше длинны доски, вернуть да
*/
bool can_place_part(int p, int b)
{
    int d, f=summ_length_parts(b);

    if(f == 0)
    {
        d = part[p].length;
    }
    else
    {
        d = f + part[p].length + disk_thickness;
    }

    if(d <= board[b].length)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
поставить деталь на доску, поставить счётчик на следующиц индекс
*/
void place_part(int p, int b)
{
    board[b].combination[board[b].part_counter] = p;
    board[b].part_counter++;
}

/*
декрементировать счётчик, обнулить позицию
*/
void remove_part(int b)
{
    if(board[b].combination[0]!=0)
    {
        board[b].part_counter--;
        board[b].combination[board[b].part_counter]=0;
    }
}

/*
пробежаться по всем доскам, выбрать самый маленький обрезок
*/
int shorter_remnat()
{
    int min = board[0].old_remnat, b=0;

    for(int i=0; i<lenght_boards; i++)
    {

        if(board[i].used != 0)
        {
            if(min > board[i].old_remnat)
            {

                min = board[i].old_remnat;
                b = i;
            }
        }
    }
    return b;
}

/*
сбросить рабочую и буферную позицию, счётчик и обрезки
*/
void clear()
{
    for(int i=0; i<lenght_boards; i++)
    {
        for(int j=0; j<lenght_parts; j++)
        {
            board[i].combination[j]=0;
            board[i].parts_buffer[j]=0;
            board[i].part_counter=0;
        }
        board[i].old_remnat=10000;
    }
}

/*
ообновить значение обрезка, скопировать в буффер
рабочую комбинацию
*/
void copy_to_buffer(int b)
{

    board[b].old_remnat = board[b].length - summ_length_parts(b);

    for(int i=0; i<lenght_parts; i++)
    {
        board[b].parts_buffer[i] = board[b].combination[i];
    }
}

/*

*/
void check(int b)
{
    if(board[b].old_remnat > board[b].length - summ_length_parts(b))
    {
        copy_to_buffer(b);
    }
}


void recurs(int n, int b)
{
    bool flag=1;
    for(int i=n; i<lenght_parts; i++)
    {

        if((part[i].used != 0))
        {

            if(can_place_part(i, b))
            {
                flag=1;
                place_part(i, b);

#if EBUG_MODE
                dprint();
#endif // EBUG_MODE

                recurs(n+1, b);
                remove_part(b);
            }
            else if(flag==1)
            {
                check(b);

#if EBUG_MODE
                dprint();
#endif // EBUG_MODE

                flag=0;
            }

        }

    }
}


void copy_to_fin(int b)
{
    for(int i=0; i<lenght_parts; i++)
    {
        board[b].best_combination[i] = board[b].parts_buffer[i];

        part[board[b].best_combination[i]].used=0;
    }
    board[b].used=0;
}


void run()
{
    for(int i=0; i<lenght_boards; i++)
    {
        if(board[i].used != 0)
        {
            for(int j=0; j<lenght_boards; j++)
            {
                recurs(1, j);
            }
            copy_to_fin(shorter_remnat());
            clear();
        }
    }
}


void printmass()
{
    for(int i=0; i<lenght_boards; i++)
    {
        printf("Доска №%d (%d):", i, board[i].length);
        for(int j=0; j<lenght_parts; j++)
        {
            if(board[i].best_combination[j]!=0)
                printf(" %d;", part[board[i].best_combination[j]].length);
        }
        printf("\n");
    }

    printf("Невместившиеся детали:");
    for(int j=0; j<lenght_parts; j++)
    {
        if(part[j].used != 0)
        {
            printf(" %d;", part[j].length);
        }
    }
    printf("\n");
}


int main()
{
    #if SEND_MODE
        setlocale(CP_UTF8, "russian");
    #else
        SetConsoleOutputCP(CP_UTF8);
    #endif
    run();
    printmass();
    system("pause");
    return 0;
}
