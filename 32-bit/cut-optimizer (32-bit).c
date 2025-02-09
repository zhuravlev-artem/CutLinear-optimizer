#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>

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

/*��������� ���������*/
struct Board
{
        int length;                         /*����� ���������*/
        int counter;                        /*���������� ������� �� ���������*/
        int best_counter;           /*����������� ��������� ����������� ������� �� ���������*/
        int remnat;                         /*����� �������*/
        bool used;                          /*������� �� ���������� ��� ���� ���������?*/
        int* combination;           /*��������� �� ������, � ������� ����� ������������ ����������*/
        int* buffer;                        /*��������� �� ������, � ������� ����� ��������� ��������� ������ ����������*/
        int* best_combination;  /*��������� �� ������, � ������� ����� ��������� �������� ����������*/
};

/*��������� ������*/
struct Part
{
        int length; /*����� ������*/
        bool used;  /*������������ �� ��� � �������� ����������?*/
};

/*��������� �������� �������� ������*/
struct Node
{
        int data;
        struct Node* next;
};

/*������� �������*/
int blade_thickness;

/*���������� ������� � �����*/
int length_parts = 1;
int length_boards = 0;

/*������ ����������� ������*/
int char_buffer = 0;

struct Part* part = NULL;
struct Board* board = NULL;

/*��������� ������� ������ ��� ������� � ���������*/
struct Node* parts_list = NULL;
struct Node* boards_list = NULL;

/*��������� ������� �������� ������*/
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

/*�������� ������ ��� ������ �������� �������� ������*/
struct Node* new_node(int Data)
{
        struct Node* node = (struct Node*)malloc(sizeof(struct Node));
        node->data=Data;
        node->next=NULL;
        return node;
}

/*�������� �������� � ����� ������*/
void n_append(struct Node* Hd, int Data)
{
        struct Node* node_ptr = Hd;

        while(node_ptr->next != NULL)
                node_ptr = node_ptr->next;

        node_ptr->next = new_node(Data);
}

/*���������� ������, ���������� ��� ������� ������*/
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


#if DEBUG_MODE
/*����������� ������� ������*/
void n_print(struct Node* Hd)
{
        printf("\n");

        for(int i=0; n_read(Hd, i) != NULL; i++)
                printf("%d->", n_read(Hd, i)->data);

        printf("NULL\n");
}

/*����������� ��������� ��������� "������"*/
void Pprint(struct Part x)
{
        printf("\tlength: %d\n\tused: %d\n", x.length, x.used);
}

/*����������� ��������� ��������� "���������"*/
void Bprint(struct Board x)
{
        printf("\tlength: %d\n\tcounter: %d\n\tremnat: %d\n\tused: %d\n\tcombination:", x.length, x.counter, x.remnat, x.used);
        for(int i=0; i<length_parts; i++)
                printf(" %d,", x.combination[i]);

        printf("\b\n\tbuffer:");

        for(int i=0; i<length_parts; i++)
                printf(" %d,", x.buffer[i]);

        printf("\b\n\tbest_combination:");

        for(int i=0; i<length_parts; i++)
                printf(" %d,", x.best_combination[i]);

        printf("\b\n");
}
#endif

#if DEBUG_MODE_ALL
/*����������� ��������� ���� ��������*/
void Aprint()
{
        printf("Main plan:\n");
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
                printf("rremnat: %d\t", board[i].remnat);
                printf("counter: %d\t", board[i].counter);
                printf("best counter: %d\n", board[i].best_counter);
        }

        printf("\nbuffer:\n");

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

        printf("\nfin:\n");

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
                        printf("Unused");
                else
                        printf("Used");

                printf("\n");
        }

        printf("\nare used:\t");

        for(int i=1; i<length_parts; i++)
                printf("%d:%d; ", part[i].length, part[i].used);

        printf("\n");

        system("pause");
}
#endif

/*��������� ������, ���������� ����� ��������������
�� ��������� �������� � ������ ������ �������*/
int summ_length_parts(int idboard)
{
        int buffer = 0;
        for(int i = 0; i < length_parts; i++)
        {
                buffer += part[board[idboard].combination[i]].length;

                if( (i > 0) && (board[idboard].combination[i] != 0) )
                        buffer += blade_thickness;
        }
        return buffer;
}

/*��������� ���������� �� ����� ��������� �� ����� n-� ������*/
int end_to_end_summ_length_parts(int position, int idboard)
{
        int buffer = 0;
        for(int i = 0; i < position+1; i++)
        {
                buffer += part[board[idboard].best_combination[i]].length;

                if( i > 0 && (board[idboard].best_combination[i] != 0) )
                        buffer += blade_thickness;
        }
        return buffer;
}

/*��������: ����� �� �� ����������� �����������
������ �� ������������ ���������?*/
bool can_place_part(int idpart, int idboard)
{
        if(part[idpart].used == USED){return FALSE;}

        int buffer, summ = summ_length_parts(idboard);

        if(summ == 0)
                buffer = part[idpart].length;
        else
                buffer = part[idpart].length + summ + blade_thickness;

        if(buffer <= board[idboard].length)
                return TRUE;
        else
                return FALSE;
}

/*���������� ������ �� ���������*/
void place_part(int idpart, int idboard)
{
        board[idboard].combination[board[idboard].counter] = idpart;
        board[idboard].counter++;
}

/*������ ������ � ���������*/
void remove_last_part(int idboard)
{
        if(board[idboard].combination[0]!=0)
        {
                board[idboard].counter--;
                board[idboard].combination[board[idboard].counter]=0;
        }
}

/*����� ����� �������� ������� �� ��������� (� "������")*/
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

/*�������� ����� � ���� ��������*/
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

/*����������� ������������ ���������� � �����*/
void copy_to_buffer(int idboard)
{
        board[idboard].remnat = board[idboard].length - summ_length_parts(idboard);
        board[idboard].best_counter = board[idboard].counter;

        for(int i=0; i<length_parts; i++)
                board[idboard].buffer[i] = board[idboard].combination[i];
}

/*��������� ������������ ���������� � ����������� � � �����,
���� ��� ����������� ��� ������ � ������*/
void check(int idboard)
{
        int remnat = board[idboard].length - summ_length_parts(idboard);

        bool A = (remnat == board[idboard].remnat);
        bool E = A && (board[idboard].counter < board[idboard].best_counter);

        if(remnat < board[idboard].remnat)
                copy_to_buffer(idboard);
        else if(E)
                copy_to_buffer(idboard);
}

/*��� �� ������ ������������?*/
bool all_parts_is_used()
{
        for(int i=0; i<length_parts; i++)
                if(part[i].used == UNUSED) return FALSE;

        return TRUE;
}

/*������ ������� �� ����� ���������*/
void recurs(int n, int idboard)
{
        int prev_length = -1;

        for(int i=n; i<length_parts; i++)
        {
                if(can_place_part(i, idboard))
                {
                        if(part[i].length != prev_length)
                        {
                                place_part(i, idboard);
                                prev_length = part[i].length;
#if DEBUG_MODE_ALL
printf("place_part\n");
Aprint();
#endif
                                check(idboard);
#if DEBUG_MODE_ALL
printf("check\n");
Aprint();
#endif
                                recurs(i+1, idboard);

                                remove_last_part(idboard);
#if DEBUG_MODE_ALL
printf("remove_last_part\n");
Aprint();
#endif
                        }
                }
        }
}

/*����������� ���������� �� ������ �� �������� ���������*/
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

/*������� �� ������ ������?*/
bool is_letter(int c){
        return (!(c >= '0' && c <= '9') && !(c == ' ' || c == '\t'));
}

/*�������� �� ������ ������?*/
bool is_number(int c){
        return (c >= '0' && c <= '9');
}

/*�������� �� ������ ����������?*/
bool is_space(int c){
        return (c == ' ' || c == '\t');
}

/*��������� ���������� ����� � ��������� ������� ������*/
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

/*�������� ����� �� ����� �����*/
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

/*��������� �� ������� '\n' */
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

/*���� ���� ��������� � �������*/
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

/*��������� ����������� ���� � ����� (��� ��������� �������)*/
int digit_count(int number)
{
        int i;
        for(i=0; number != 0; i++)
                number /= 10;

        return i;
}

/*��������� ������� �����������*/
void optimize()
{
        for(int i=0; i<length_boards && !all_parts_is_used(); i++)
        {
                for(int j=0; j<length_boards; j++)
                {
                        if(board[j].used == UNUSED)
                                recurs(1, j);
                }
                /*�� ���� ���������� ���������� � ���������� ��������,*/
                /*���������� ���������� �������������� ������*/
                copy_to_fin(shorter_remnat());

                /*�������� ����� ������ ���������� � �������� ����� ��������*/
                clear();
        }/*��������� ��� ��������� ���������*/
}

/*����������� �������� ���������� ��� ���� ���������*/
void print_combin()
{
        printf("Cutting plan:\n");

        for(int i=0; i<length_boards; i++)
        {
                printf("\nBillet #%d (%d)\nParts:     ", i+1, board[i].length);

                for(int j=0; j<length_parts; j++)
                {
                        int a = board[i].best_combination[j];
                        int c = end_to_end_summ_length_parts(j, i);

                        if(a!=0)
                        {
                                for(int k=0; k<digit_count(c)-digit_count(part[a].length); k++)
                                        printf(" ");

                                printf(" %d;", part[a].length);
                        }
                }

        printf("\nFrom start:");

        for(int j=0; j<length_parts; j++){
                if(board[i].best_combination[j]!=0){
                        printf(" %d;", end_to_end_summ_length_parts(j, i));
                }
        }

        printf("\n");
        }

        printf("\n");
        if(all_parts_is_used())
                printf("All parts used");
        else
        {
                printf("Unused parts:");

                for(int i=0; i<length_parts; i++)
                {
                        if(part[i].used == UNUSED)
                                printf(" %d;", part[i].length);
                }
        }
        printf("\n\n");
}

/*��������� ���������� ����� � ��������� ����*/
void save_print(FILE* fptr)
{
    fprintf(fptr, "Cutting plan:\n");

        for(int i=0; i<length_boards; i++){
                fprintf(fptr, "\nBillet #%d (%d)\nParts:     ", i+1, board[i].length);

                for(int j=0; j<length_parts; j++){
                         int a = board[i].best_combination[j];
                         int c = end_to_end_summ_length_parts(j, i);


                        if(a!=0){
                                for(int k=0; k<digit_count(c)-digit_count(part[a].length); k++){
                                        fprintf(fptr, " ");
                                }
                                fprintf(fptr, " %d;", part[a].length);
                        }
                }

                fprintf(fptr, "\nFrom start:");

                for(int j=0; j<length_parts; j++){
                        if(board[i].best_combination[j]!=0){
                                fprintf(fptr, " %d;", end_to_end_summ_length_parts(j, i));
                        }
                }

                fprintf(fptr, "\n");
        }

        fprintf(fptr, "\n");
        if(all_parts_is_used()){
                fprintf(fptr, "All parts used");
        }else{
                fprintf(fptr, "Unused parts:");

                for(int i=0; i<length_parts; i++){
                        if(part[i].used == UNUSED){
                                fprintf(fptr, " %d;", part[i].length);
                        }
                }
        }
        fprintf(fptr, "\n\n");
        printf("Plan is saved\n");
}


int main()
{
        /*���������� ��������� utf-8 � ������� ��� ������ ��������*/
        SetConsoleOutputCP(CP_UTF8);

        FILE* finput = fopen("input.txt", "r");
        FILE* output = fopen("output.txt", "w+");

        if((finput != NULL) && (output != NULL))
        {
                /*��� ����� ������� � ���������, ���������
                ������� "�����������" ������� � ���������,
                � �������� ������ �������� ���������*/
                input(finput);
                optimize();

                print_combin();
                save_print(output);

                fclose(finput);
                fclose(output);
        }
        else
        {
                if((finput == NULL) && (output == NULL))
                        printf("Failed to open input.txt and output.txt files\n");
                else if(finput == NULL)
                        printf("Failed to open input.txt file\n");
                else
                        printf("Failed to open output.txt file\n");
        }

        system("pause");
        return 0;
}

