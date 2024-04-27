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

/*толщина пропила*/
int blade_thickness;

/*количество деталей и досок*/
int length_parts = 1;
int length_boards = 0;

struct Node
{
        int data;
        struct Node* next;
};

struct Part* part = NULL;
struct Board* board = NULL;

/*прочитать элемент связного списка*/
struct Node* n_read(struct Node* Hd, unsigned int id)
{
        struct Node* ptr = Hd;
        int i=0;
        for(; (i < id) && (ptr->next != NULL); i++){
                ptr = ptr->next;
        }

        if(i == id){
                return ptr;
        }else{
                return NULL;
        }
}

/*выделить память для нового элемента связного списка*/
struct Node* new_node(int Data)
{
        struct Node* node = (struct Node*)malloc(sizeof(struct Node));
        node->data=Data;
        node->next=NULL;
        return node;
}

/*добавить значение в конец списка*/
void n_append(struct Node* Hd, int Data)
{
        struct Node* node_ptr=Hd;

        while(node_ptr->next != NULL)
                node_ptr = node_ptr->next;

        node_ptr->next = new_node(Data);
}

/*освободить память, выделенную под связный список*/
void n_free(struct Node* Hd)
{
        struct Node *now_ptr = Hd, *next_ptr = now_ptr->next;

        free(now_ptr);

        while(next_ptr != NULL)
        {
                now_ptr = next_ptr;
                next_ptr = now_ptr->next;

                free(now_ptr);
        }
}

/*распечатать связный список*/
void n_print(struct Node* Hd)
{
        printf("\n");

        for(int i=0; n_read(Hd, i) != NULL; i++)
                printf("%d->", n_read(Hd, i)->data);

        printf("NULL\n");
}

/*(не)безопасный ввод целых чисел*/
int fscan_uint(FILE* fp)
{

                start:
        while(TRUE)
        {

                char str[256];
                unsigned int result = 0;
                char simbol = 0;

                fscanf(fp, "%s", str);

                for(int i = 0; str[i] != '\0'; i++)
                {
                        simbol = str[i];

                        if(simbol >= '0' && simbol <= '9')
                        {
                                result *= 10;
                                result += simbol - '0';
                        }
                        else
                                goto start;
                }
                return result;
        }
}

/*элемент консольного интерфейса для ввода длин заготовок и деталей*/
void input(FILE* fp)
{
        int scanb;

        struct Node* boards_list = (struct Node*)calloc(sizeof(struct Node), 1);
        struct Node* parts_list = (struct Node*)calloc(sizeof(struct Node), 1);

        while(TRUE)
        {
                scanb = fscan_uint(fp);
                if(scanb == 0) break;

                n_append(parts_list, scanb);
                length_parts++;
        }

#if DEBUG_MODE && DEBUG_MODE_ALL
        n_print(parts_list);
#endif

        part = (struct Part*) malloc( sizeof(struct Part) * length_parts);

        for(int i = 0; i<length_parts; i++)
        {
                part[i].length = n_read(parts_list, i)->data;
                part[i].used = UNUSED;

#if DEBUG_MODE && DEBUG_MODE_ALL
                Pprint(part[i]);
#endif
        }

        n_free(parts_list);

        boards_list->data = fscan_uint(fp);
        boards_list->next = NULL;
        length_boards++;

        while(TRUE)
        {
                scanb = fscan_uint(fp);
                if(scanb==0) break;

                n_append(boards_list, scanb);
                length_boards++;
        }


#if DEBUG_MODE && DEBUG_MODE_ALL
        n_print(boards_list);
#endif

        board = (struct Board*)malloc(sizeof(struct Board)*length_boards);

        for(int i = 0; i<length_boards; i++)
        {
                board[i].length = n_read(boards_list, i)->data;
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

        n_free(boards_list);

        blade_thickness = fscan_uint(fp);
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
                printf("№%d\t%d\t%d\n", i+1, part[i].length, part[i].used);
        }
        printf("\n\n");
}


int main()
{
        /*установить кодировку utf-8 в консоли для вывода кирилицы*/
        SetConsoleOutputCP(CP_UTF8);

        FILE* finput = fopen("input.txt", "r");

        if(finput)
        {
                /*при вводе деталей и заготовок, создаются
                массивы "экземпляров" деталей и заготовок,
                с которыми дальше работает программа*/
                input(finput);

                print_combin();

                fclose(finput);
        }
        else
        {
                printf("Не удалось открыть файл input.txt\n");
        }

        system("pause");
        return 0;
}

