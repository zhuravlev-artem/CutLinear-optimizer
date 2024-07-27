
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
int blade_thickness = 0;

/*количество деталей и досок*/
int length_parts = 1;
int length_boards = 0;

/*хранит считываемый символ*/
int char_buffer = 0;

struct Part* part = NULL;
struct Board* board = NULL;

/*буферные связные списки для деталей и заготовок*/
struct Node* parts_list = NULL;
struct Node* boards_list = NULL;


