#ifndef INPUT_OUTPUT
#define INPUT_OUTPUT

/*структура элемента связного списка*/
struct Node
{
        int data;
        struct Node* next;
};

/*хранит считываемый символ*/
extern int char_buffer;

/*буферные связные списки для деталей и заготовок*/
extern struct Node* parts_list;
extern struct Node* boards_list;

/*прочитать элемент связного списка*/
struct Node* n_read(struct Node* Hd, unsigned int id);

/*выделить память для нового элемента связного списка*/
struct Node* new_node(int Data);

/*добавить значение в конец списка*/
void n_append(struct Node* Hd, int Data);

/*освободить память, выделенную под связный список*/
void n_free(struct Node* Hd);

/*ввод длин заготовок и деталей*/
void input(FILE* fp);

#if DEBUG_MODE
/*распечатать связный список*/
void n_print(struct Node* Hd);
#endif //DEBUG_MODE

/*вычислить расстояние от торца заготовки до конца n-й детали*/
int end_to_end_summ_length_parts(int position, int idboard);

/*сосчитать колличество цифр в цисле (для красивого отступа)*/
int digit_count(int number);

/*распечатать итоговую комбинацию для всех заготовок*/
void print_combin();

/*сохранить консольный вывод в текстовый файл*/
void save_print(FILE* fptr);
#endif //INPUT_OUTPUT
