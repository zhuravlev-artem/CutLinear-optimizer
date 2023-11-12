#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <locale.h>
#include <stdlib.h>
#define USED 1
#define UNUSED 0
#define TRUE 1
#define FALSE 0


#define DEBUG_MODE 0
#define DEBUG_MODE_ALL 0
#define SEND_MODE 1



struct Board
{
    int length;
    int counter;
    int best_counter;
    int remnat;
    bool used;
    int* combination;
    int* buffer;
    int* best_combination;
};

struct Part
{
    int length;
    bool used;
};

struct Node
{
    int data;
    struct Node* next;
};

struct Part* part = NULL;
struct Board* board = NULL;

int blade_thickness;
int length_parts=1;
int length_boards=0;


struct Node* n_read(struct Node* Hd, unsigned int id)
{
    struct Node* ptr = Hd;
    int i=0;
    for(; (i < id)&&(ptr->next != NULL); i++)
    {
        ptr=ptr->next;
    }
    if(i==id){return ptr;}
    else{return NULL;}
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

void n_free(struct Node* Hd)
{
    struct Node **now_ptr = &Hd,
                *next_ptr = (*now_ptr)->next;

        free(*now_ptr);

    while(next_ptr != NULL)
    {
        (*now_ptr) = next_ptr;
        next_ptr = (*now_ptr)->next;

        free(*now_ptr);
    }
}

#if DEBUG_MODE && DEBUG_MODE_ALL
void n_print(struct Node* Hd)
{
    printf("\n");
    for(int i=0; n_read(Hd, i) != NULL; i++){
        printf("%d->", n_read(Hd, i)->data);
    }

    printf("NULL\n");
}

void Pprint(struct Part x)
{
    printf("\tlength: %d\n\tused: %d\n", x.length, x.used);
}

void Bprint(struct Board x)
{
    printf("\tlength: %d\n\tcounter: %d\n\tremnat: %d\n\tused: %d\n\tcombination:", x.length, x.counter, x.remnat, x.used);
    for(int i=0; i<length_parts; i++)
    {
        printf(" %d,", x.combination[i]);
    }
    printf("\b\n\tbuffer:");

    for(int i=0; i<length_parts; i++)
    {
        printf(" %d,", x.buffer[i]);
    }
    printf("\b\n\tbest_combination:");

    for(int i=0; i<length_parts; i++)
    {
        printf(" %d,", x.best_combination[i]);
    }
    printf("\b\n");
}
#endif

#if DEBUG_MODE
void dprint()
{

    printf("основная раскладка:\n");
    for(int i=0; i<length_boards; i++)
    {
        printf("(%d):\t", board[i].length);
        for(int j=0; j<length_parts; j++)
        {
            if(part[board[i].combination[j]].length != 0)
                printf("%d;\t", part[board[i].combination[j]].length);
            else
                printf("\t");
        }
        printf("обрезок: %d\t", board[i].remnat);
        printf("счётчик: %d\t", board[i].counter);
        printf("лучший счётчик: %d\n", board[i].best_counter);
    }
    printf("\n");


    printf("буфер:\n");
    for(int i=0; i<length_boards; i++)
    {
        printf("(%d):\t", board[i].length);
        for(int j=0; j<length_parts; j++)
        {
            if(part[board[i].buffer[j]].length != 0)
                printf("%d;\t", part[board[i].buffer[j]].length);
            else
                printf("\t");
        }
        printf("\n");
    }
    printf("\n");


    printf("итог:\n");
    for(int i=0; i<length_boards; i++)
    {
        printf("(%d):\t", board[i].length);

        for(int j=0; j<length_parts; j++)
        {
            if(part[board[i].best_combination[j]].length != 0)
                printf("%d;\t", part[board[i].best_combination[j]].length);
            else
                printf("\t");
        }

        if(board[i].used==UNUSED)
        {
            printf("Неиспользована");
        } else {
            printf("использована");
        }
        printf("\n");
    }
    printf("\n");

    printf("используются:\t");
    for(int i=1; i<length_parts; i++)
    {
        printf("%d:%d; ", part[i].length, part[i].used);
    }
    printf("\n");

    system("pause");
}
#endif

int summ_length_parts(int idboard)
{
    int buffer = 0;
    for(int i = 0; i < length_parts; i++)
    {

        buffer += part[board[idboard].combination[i]].length;

        if( (i > 0) && (board[idboard].combination[i] != 0) )
        {
            buffer += blade_thickness;
        }
    }
    return buffer;
}

int end_to_end_summ_length_parts(int position, int idboard)
{
    int buffer = 0;
    for(int i = 0; i < position+1; i++)
    {

        buffer += part[board[idboard].best_combination[i]].length;

        if( (i > 0) && (board[idboard].best_combination[i] != 0) )
        {
            buffer += blade_thickness;
        }
    }
    return buffer;
}

bool can_place_part(int idpart, int idboard)
{
    if(part[idpart].used == USED){return FALSE;}

    int buffer, summ = summ_length_parts(idboard);

    if(summ == 0)
        {buffer = part[idpart].length;}
    else
        {buffer = part[idpart].length + summ + blade_thickness;}

    if(buffer <= board[idboard].length)
        {return TRUE;}
    else
        {return FALSE;}
}

void place_part(int idpart, int idboard)
{
    board[idboard].combination[board[idboard].counter] = idpart;
    board[idboard].counter++;
}

void remove_last_part(int idboard)
{
    if(board[idboard].combination[0]!=0)
    {
        board[idboard].counter--;
        board[idboard].combination[board[idboard].counter]=0;
    }
}

int shorter_remnat()
{
    int min = 10000, buffer=0;

    for(int i=0; i<length_boards; i++)
    {
        if(board[i].used == UNUSED)
        {
            if(board[i].remnat < min)
            {

                min = board[i].remnat;
                buffer = i;
            }
        }
    }
    return buffer;
}

void clear()
{
    for(int i=0; i<length_boards; i++)
    {
        for(int j=0; j<length_parts; j++)
        {
            board[i].combination[j] = 0;
            board[i].buffer[j] = 0;
        }
        board[i].best_counter = 10000;
        board[i].counter = 0;
        board[i].remnat = 10000;
    }
}

void copy_to_buffer(int idboard)
{
    board[idboard].remnat = board[idboard].length - summ_length_parts(idboard);
    board[idboard].best_counter = board[idboard].counter;

    for(int i=0; i<length_parts; i++)
    {
        board[idboard].buffer[i] = board[idboard].combination[i];
    }
}

void check(int idboard)
{
    int remnat = board[idboard].length - summ_length_parts(idboard);

    if(remnat < board[idboard].remnat)
    {
        copy_to_buffer(idboard);
    }
    else if( (remnat == board[idboard].remnat) && (board[idboard].counter < board[idboard].best_counter) )
    {
        copy_to_buffer(idboard);
    }
}

bool all_is_used()
{
    for(int i=0; i<length_parts; i++){
        if(part[i].used == UNUSED)
        {
            return FALSE;
        }
    }
    return TRUE;
}

int last_unused()
{
    int buffer = 0;

    for(int i=0; i<length_parts; i++){
        if(part[i].used == UNUSED)
        {
            buffer = i;
        }
    }
    return buffer;
}

void recurs(int n, int idboard)
{
    for(int i=n; i<length_parts; i++)
    {
        if(can_place_part(i, idboard))
        {
            place_part(i, idboard);
#if DEBUG_MODE
printf("\nPlaced\n");
dprint();
#endif
            check(idboard);
#if DEBUG_MODE
printf("\nChecked\n");
dprint();
#endif
            if(i+1 < length_parts){
                recurs(i+1, idboard);
            }

            remove_last_part(idboard);
#if DEBUG_MODE
printf("\nRemoved\n");
dprint();
#endif
        }
    }
}

void copy_to_fin(int idboard)
{
    if(board[idboard].buffer[0] != 0)
    {
        for(int i=0; i<length_parts; i++)
        {
            board[idboard].best_combination[i] = board[idboard].buffer[i];

            part[board[idboard].best_combination[i]].used=USED;
        }
        board[idboard].used=USED;
    }
}

void inputs()
{
    int scanb;
    int d = 1;

    struct Node* boards = (struct Node*)calloc(sizeof(struct Node), 1);
    struct Node* parts = (struct Node*)calloc(sizeof(struct Node), 1);

    while(TRUE)
    {
        #if SEND_MODE
        printf("Detail #%d: ", d);
        #else
        printf("Деталь №%d: ", d);
        #endif

        scanf("%d", &scanb);
        if(scanb==0){break;}

        n_append(parts, scanb);
        length_parts++;
        d++;
    }

    printf("\n");

    d = 1;

    #if SEND_MODE
    printf("Billet #%d: ", d);
    #else
    printf("Заготовка №%d: ", d);
    #endif

    scanf("%d", &boards->data);
    boards->next = NULL;
    length_boards++;
    d++;

    while(TRUE)
    {
        #if SEND_MODE
        printf("Billet #%d: ", d);
        #else
        printf("Заготовка №%d: ", d);
        #endif

        scanf("%d", &scanb);
        if(scanb==0){break;}

        n_append(boards, scanb);
        length_boards++;
        d++;
    }
#if SEND_MODE
    printf("\nKerf width: ");
#else
    printf("\nТолщина пилы: ");
#endif

    scanf("%d", &blade_thickness);

#if DEBUG_MODE && DEBUG_MODE_ALL
n_print(parts);
n_print(boards);
#endif

    part = (struct Part*)malloc(sizeof(struct Part)*length_parts);

    for(int i = 0; i<length_parts; i++)
    {
        part[i].length = n_read(parts, i)->data;
        part[i].used = UNUSED;
#if DEBUG_MODE && DEBUG_MODE_ALL
Pprint(part[i]);
#endif
    }

    n_free(parts);

    board = (struct Board*)malloc(sizeof(struct Board)*length_boards);

    for(int i = 0; i<length_boards; i++)
    {
        board[i].length = n_read(boards, i)->data;
        board[i].counter = 0;
        board[i].best_counter = 10000;
        board[i].remnat = 10000;
        board[i].used = UNUSED;
        board[i].combination = calloc(length_parts, sizeof(int));
        board[i].buffer = calloc(length_parts, sizeof(int));
        board[i].best_combination = calloc(length_parts, sizeof(int));
#if DEBUG_MODE && DEBUG_MODE_ALL
Bprint(board[i]);
#endif
    }

    n_free(boards);
}

void printmass()
{
#if SEND_MODE
    printf("\n-------------------------------------------------\nCutting layout\n");
#else
    printf("\n-------------------------------------------------\nРаскладка\n");
#endif

    for(int i=0; i<length_boards; i++)
    {
#if SEND_MODE
        printf("\nBillet #%d (%d):", i+1, board[i].length);
#else
        printf("\nЗаготовка №%d (%d):", i+1, board[i].length);
#endif

        for(int j=0; j<length_parts; j++)
        {
            if(board[i].best_combination[j]!=0)
                printf(" %d;", part[board[i].best_combination[j]].length);
        }
    }

    printf("\n\n");
    if(all_is_used())
    {
#if SEND_MODE
        printf("All parts are complete");
#else
        printf("Все детали распределены");
#endif
    }
    else
    {
#if SEND_MODE
        printf("Incomplete parts:");
#else
        printf("Невместившиеся детали:");
#endif
        for(int i=0; i<length_parts; i++)
        {
            if(part[i].used == UNUSED)
            {
                printf(" %d;", part[i].length);
            }
        }
    }
        printf("\n\n");
}

void printmass_for_rule()
{

    printf("-------------------------------------------------");

    #if SEND_MODE
    printf("\nDistances from the end face of the board to the end face of the nth part\n");
    #else
    printf("\nРасстояния от торца доски до конца n-ой детали\n");
    #endif
    for(int i=0; i<length_boards; i++)
    {
        #if SEND_MODE
        printf("\nBillet #%d (%d):", i+1, board[i].length);
        #else
        printf("\nЗаготовка №%d (%d):", i+1, board[i].length);
        #endif

        for(int j=0; j<length_parts; j++)
        {
            if(board[i].best_combination[j]!=0)
                printf(" %d;", end_to_end_summ_length_parts(j, i));
        }
    }

    printf("\n\n");
}

void run()
{
    for(int i=0; (i<length_boards) && (all_is_used() == FALSE); i++)
    {

#if DEBUG_MODE && DEBUG_MODE_ALL
printf("\nReturn 1\n");
dprint();
#endif
        for(int j=0; j<length_boards; j++)
        {
            if(board[j].used == UNUSED)
            {
#if DEBUG_MODE && DEBUG_MODE_ALL
printf("\nBoard %d is unused\n", j+1);
dprint();
#endif
                recurs(1, j);
            }
        }
#if DEBUG_MODE && DEBUG_MODE_ALL
printf("\nReturn 2\n");
dprint();
#endif
        copy_to_fin(shorter_remnat());

#if DEBUG_MODE
printf("\nCopied\n");
dprint();
#endif
        clear();

#if DEBUG_MODE
printf("\nCleared\n");
dprint();
#endif
    }
}


int main()
{
    #if SEND_MODE
    printf("Enter the lengths of the parts and boards. Enter 0 to finish the input process\n\n");
    #else
    SetConsoleOutputCP(CP_UTF8);
    #endif

    inputs();
    run();

    printmass();
    printmass_for_rule();
    system("pause");


    return 0;
}

