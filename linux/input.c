#include "defines_structs.h"

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
        struct Node* node_ptr = Hd;

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

/*являетя ли символ буквой?*/
bool is_letter(int c)
{
        return (!(c >= '0' && c <= '9') && !(c == ' ' || c == '\t'));
}

/*является ли символ числом?*/
bool is_number(int c)
{
        return (c >= '0' && c <= '9');
}

/*является ли символ пробельным?*/
bool is_space(int c)
{
        return (c == ' ' || c == '\t');
}

/*добавлять получаемые длины в буфферные связные списки*/
void addls(int* mode, int len, int cnt)
{
        int* counter = NULL;
        struct Node* Hd_node = NULL;

        if(*mode == M_PARTS)
        {
                counter = &length_parts;
                Hd_node = parts_list;

                if(len == 0 && cnt == -1)
                {
                        *mode = M_BOARDS;
                        return;
                }
        }
        else if(*mode == M_BOARDS)
        {
                counter = &length_boards;
                Hd_node = boards_list;

                if(len == 0 && cnt == -1)
                {
                        *mode = M_BLADE;
                        return;
                }
        }
        else if(*mode == M_BLADE)
        {
                blade_thickness = len;
                *mode = M_NOP;
                return;
        }

        if(*mode != M_NOP){
                if(cnt == -1) cnt = 1;

                for(int i=0; i<cnt; i++)
                {
                        n_append(Hd_node, len);
                        (*counter)++;
                }
        }
}

/*получить число из файла ввода*/
void getnumber(FILE* fp, int* num, int* err)
{
        *num = 0;
        int number = 0;
        int sub_mode = SM_OUT;

        while(TRUE)
        {
                char_buffer = fgetc(fp);
                if(is_space(char_buffer))
                {
                        if(sub_mode == SM_IN){
                                *num = number;
                                *err = ERR_GOOD;
                                return;
                        }
                }
                else if(is_number(char_buffer))
                {
                        if(sub_mode == SM_OUT)
                                sub_mode = SM_IN;

                        number *= 10;
                        number += char_buffer -'0';
                }
                else if(is_letter(char_buffer))
                {
                        if(char_buffer == '\n')
                        {
                                if(sub_mode == SM_IN){
                                        *num = number;
                                        *err = ERR_SLN;
                                        return;
                                }
                                else if(sub_mode == SM_OUT){
                                        *err = ERR_NULL;
                                        return;
                                }
                        }
                        if(char_buffer == EOF){
                                *num = number;
                                *err = ERR_EOF;
                                return;
                        }else{
                                *err = ERR_LETTER;
                                return;
                        }
                }
        }
        *err = ERR_UNKNOW;
        return;
}

/*промотать до символа '\n' */
void skip(FILE* fp, int* err)
{
        while(TRUE)
        {
                char_buffer = fgetc(fp);
                if(char_buffer == '\n')
                {
                        return;
                }
                else if(char_buffer == EOF)
                {
                        *err = ERR_EOF;
                        return;
                }
        }
}

/*ввод длин заготовок и деталей*/
void input(FILE* fp)
{
        boards_list = (struct Node*)calloc(sizeof(struct Node), 1);
        boards_list->next = NULL;
        parts_list = (struct Node*)calloc(sizeof(struct Node), 1);
        parts_list->next = NULL;

        int length;
        int count;
        int err;
        int mode = M_PARTS;

        while(TRUE)
        {
                start:

                length = 0;
                count = 0;
                err = 0;

                getnumber(fp, &length, &err);

                if(err == ERR_SLN) count = -1;
                else if(err == ERR_LETTER)
                {
                        skip(fp, &err);
                        if (err == ERR_EOF) break;
                        goto start;
                }
                else if(err == ERR_NULL) goto start;
                else if((err == ERR_GOOD || err == ERR_SLN || err == ERR_EOF) && length == 0)
                {
                        addls(&mode, 0, -1);

                        if(err == ERR_EOF) break;

                        if(err != ERR_SLN)
                        {
                                skip(fp, &err);
                                if (err == ERR_EOF) break;
                        }

                        goto start;
                }
                else if(err == ERR_EOF) {
                        addls(&mode, length, count);////
                        break;
                }

                if(err != ERR_SLN && err != ERR_EOF){
                        getnumber(fp, &count, &err);

                        if(err == ERR_LETTER) count = -1;
                        else if(err == ERR_EOF) {
                                addls(&mode, length, count);////
                                break;
                        }

                        if(err != ERR_SLN && err != ERR_NULL)
                        {
                                skip(fp, &err);
                                if (err == ERR_EOF) break;
                        }
                }

                addls(&mode, length, count);////
        }

        part = (struct Part*) malloc( sizeof(struct Part) * length_parts);
        board = (struct Board*)malloc(sizeof(struct Board)*length_boards);

        for(int i = 0; i<length_parts; i++)
        {
                part[i].length = n_read(parts_list, i)->data;
                part[i].used = UNUSED;
        }

        for(int i = 0; i<length_boards; i++)
        {
                board[i].length = n_read(boards_list, i+1)->data;
                board[i].counter = 0;
                board[i].best_counter = 10000;
                board[i].remnat = 10000;
                board[i].used = UNUSED;
                board[i].combination = calloc(length_parts, sizeof(int));
                board[i].buffer = calloc(length_parts, sizeof(int));
                board[i].best_combination = calloc(length_parts, sizeof(int));
        }
#if DEBUG_MODE
n_print(parts_list);
printf("\n%d\n", length_parts);
n_print(boards_list);
printf("\n%d\n", length_boards);
printf("\n%d\n", blade_thickness);
#endif
        n_free(parts_list);
        n_free(boards_list);
}

#if DEBUG_MODE
/*распечатать связный список*/
void n_print(struct Node* Hd)
{
        printf("\n");

        for(int i=0; n_read(Hd, i) != NULL; i++)
                printf("%d->", n_read(Hd, i)->data);

        printf("NULL\n");
}
#endif //DEBUG_MODE
