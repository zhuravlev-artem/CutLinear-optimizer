#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <locale.h>

struct Board
{
    int length;
    int part_counter;
    int old_remnat;
    bool used;
    int* combination;
    int* parts_buffer;
    int* best_combination;
};

struct Part
{
    int length;
    bool used;
};

int disk_thickness;
int lenght_parts;
int lenght_boards;

int main(){
    struct Board board={6, 0, 100, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};
    struct Part part = {10, 0};
    return 0;
}

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
void place_part(int p, int b)
{
    board[b].combination[board[b].part_counter] = p;
    board[b].part_counter++;
}
void remove_part(int b)
{
    if(board[b].combination[0]!=0)
    {
        board[b].part_counter--;
        board[b].combination[board[b].part_counter]=0;
    }
}
int shorter_remnat()
{
    int min = board[0].old_remnat, b=0;

    for(int i=0; i<lenght_boards; i++)
    {

        if(board[i].used != 1)
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
void copy_to_buffer(int b)
{

    board[b].old_remnat = board[b].length - summ_length_parts(b);

    for(int i=0; i<lenght_parts; i++)
    {
        board[b].parts_buffer[i] = board[b].combination[i];
    }
}
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

        if((part[i].used != 1))
        {

            if(can_place_part(i, b))
            {
                flag=1;
                place_part(i, b);

#if EBUG_MODE
                dprint();
#endif

                recurs(n+1, b);
                remove_part(b);
            }
            else if(flag)
            {
                check(b);

#if EBUG_MODE
                dprint();
#endif

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
        part[board[b].best_combination[i]].used=1;
    }
    board[b].used=1;
}
void run()
{
    for(int i=0; i<lenght_boards; i++)
    {
        if(board[i].used != 1)
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
        if(part[j].used != 1)
        {
            printf(" %d;", part[j].length);
        }
    }
    printf("\n");
}
