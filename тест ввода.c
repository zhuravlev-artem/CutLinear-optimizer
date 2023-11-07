#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <locale.h>
#include <stdlib.h>

#define EBUG_MODE 0
#define SEND_MODE 0

int disk_thickness;
int length_parts=0;
int length_boards=0;

struct Part* part = NULL;
struct Board* board = NULL;

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
#if DEBUG_MODE
void Pprint(struct Part* x)
{
    printf("\tlength: %d\n\tused: %d", x->length, x->used);
}

void Bprint(struct Board* x)
{
    printf("\tlength: %d\n\tpart_counter: %d\n\told_remnat: %d\n\tused: %d\n\tcombination:", x->length, x->part_counter, x->old_remnat, x->used);
    for(int i=0; i<length_boards; i++)
    {
        printf(" %d,", x->combination[i]);
    }
    printf("\b\n\tparts_buffer:");

    for(int i=0; i<length_boards; i++)
    {
        printf(" %d,", x->parts_buffer[i]);
    }
    printf("\b\n\tbest_combination:");

    for(int i=0; i<length_boards; i++)
    {
        printf(" %d,", x->best_combination[i]);
    }
    printf("\b");
}
#endif
struct Node
{
    int data;
    struct Node* next;
};

int summ_length_parts(int b)
{



    int buff=0;
    for(int i=0; i < length_parts; i++)
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

    for(int i=0; i<length_boards; i++)
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



    for(int i=0; i<length_boards; i++)
    {
        for(int j=0; j<length_parts; j++)
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

    for(int i=0; i<length_parts; i++)
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
    for(int i=n; i<length_parts; i++)
    {

        if((part[i].used != 0))
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
            else if(flag==1)
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



    for(int i=0; i<length_parts; i++)
    {
        board[b].best_combination[i] = board[b].parts_buffer[i];

        part[board[b].best_combination[i]].used=0;
    }
    board[b].used=0;
}


void run()
{



    for(int i=0; i<length_boards; i++)
    {
        if(board[i].used != 0)
        {
            for(int j=0; j<length_boards; j++)
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



    for(int i=0; i<length_boards; i++)
    {
        printf("Доска №%d (%d):", i, board[i].length);
        for(int j=0; j<length_parts; j++)
        {
            if(board[i].best_combination[j]!=0)
                printf(" %d;", part[board[i].best_combination[j]].length);
        }
        printf("\n");
    }

    printf("Невместившиеся детали:");
    for(int j=0; j<length_parts; j++)
    {
        if(part[j].used != 0)
        {
            printf(" %d;", part[j].length);
        }
    }
    printf("\n");
}

int n_read(struct Node* Hd, unsigned int id)
{
    struct Node* ptr = Hd;
    for(int i=0; (i < id)&&(ptr != NULL); i++)
    {
        ptr=ptr->next;
    }
    return ptr->data;
}

struct Node* NewNode(int Data)
{
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->data=Data;
    node->next=NULL;
    return node;
}

void n_append(struct Node* Hd, int Data)
{
    struct Node* node_ptr=Hd;

    while(node_ptr->next != NULL)
    {
        node_ptr = node_ptr->next;
    }

    node_ptr->next = NewNode(Data);

}

void n_print(struct Node* Hd)
{

    struct Node* node_ptr=Hd;

    while(node_ptr->next != NULL)
    {
        printf("%d->", node_ptr->data);
        node_ptr=node_ptr->next;
    }
    printf("%d->NULL", node_ptr->data);
}


int main()
{
    SetConsoleOutputCP(CP_UTF8);
    struct Node parts = {0, NULL};
    int scanb;
    int d = 0;
    while(1)
    {
        printf("деталь №%d: ", d);
        scanf("%d", &scanb);
        if(scanb==0)
        {
            break;
        }

        n_append(&parts, scanb);
        length_parts++;
        d++;
    }

    printf("\n");

    d = 0;
    printf("заготовка №%d: ", d);
    scanf("%d", &scanb);
    struct Node boards = {scanb, NULL};
    length_boards++;
    d++;

    while(1)
    {
        printf("заготовка №%d: ", d);
        scanf("%d", &scanb);
        if(scanb==0)
        {
            break;
        }

        n_append(&boards, scanb);
        length_boards++;
        d++;
    }

    printf("\nтолщина пилы: ");
    scanf("%d", &disk_thickness);



    struct Part* part = (struct Part*)malloc(sizeof(struct Part)*length_parts);

    for(int i = 0; i<length_parts; i++)
    {
        part[i].length = n_read(&parts, i);
        part[i].used = 0;
    }

    struct Board* board = (struct Board*)malloc(sizeof(struct Board)*length_boards);

    for(int i = 0; i<length_boards; i++)
    {
        board[i].length = n_read(&boards, i);
        board[i].part_counter = 0;
        board[i].old_remnat = 10000;
        board[i].used = 0;
        board[i].combination = calloc(length_parts, sizeof(int));
        board[i].parts_buffer = calloc(length_parts, sizeof(int));
        board[i].best_combination = calloc(length_parts, sizeof(int));
    }

    run();
    printmass();
    system("pause");

    return 0;
}
