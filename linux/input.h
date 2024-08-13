#ifndef INPUT
#define INPUT

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

/*являетя ли символ буквой?*/
bool is_letter(int c);

/*является ли символ числом?*/
bool is_number(int c);

/*является ли символ пробельным?*/
bool is_space(int c);

/*добавлять получаемые длины в буфферные связные списки*/
void addls(int* mode, int len, int cnt);

/*получить число из файла ввода*/
void getnumber(FILE* fp, int* num, int* err);

/*промотать до символа '\n' */
void skip(FILE* fp, int* err);

/*ввод длин заготовок и деталей*/
void input(FILE* fp);

#if DEBUG_MODE
/*распечатать связный список*/
void n_print(struct Node* Hd);
#endif //DEBUG_MODE

#endif //INPUT
