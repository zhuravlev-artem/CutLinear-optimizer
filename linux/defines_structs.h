#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef DEFINES_STRUCTS
#define DEFINES_STRUCTS

#define USED 1
#define UNUSED 0

#define TRUE 1
#define FALSE 0

#define M_PARTS 1
#define M_BOARDS 2
#define M_BLADE 3
#define M_NOP 4

#define SM_IN 1
#define SM_OUT 2
#define SM_LETTER 3

#define ERR_GOOD 1
#define ERR_LETTER 2
#define ERR_SLN 3
#define ERR_NULL 4
#define ERR_UNKNOW 5
#define ERR_EOF 6

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

/*структура элемента связного списка*/
struct Node
{
        int data;
        struct Node* next;
};

/*толщина пропила*/
extern int blade_thickness;

/*количество деталей и досок*/
extern int length_parts;
extern int length_boards;

/*хранит считываемый символ*/
extern int char_buffer;

extern struct Part* part;
extern struct Board* board;

/*буферные связные списки для деталей и заготовок*/
extern struct Node* parts_list;
extern struct Node* boards_list;


#endif //DEFINES_STRUCTS
