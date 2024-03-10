#include <stdio.h>
#include <windows.h>
#include <stdbool.h>

#define USED 1
#define UNUSED 0
#define TRUE 1
#define FALSE 0

#define DEBUG_MODE 0
#define DEBUG_MODE_ALL 0


/*именнованные наборы переменных для описания
заготовки, детали и элемента временной
структуры данных - односвязного списка*/
struct Board
{
    int length;             //длинна заготовки
    int counter;            //колличество деталей на заготовке
    int best_counter;       //минимальное временное колличество деталей на заготовке
    int remnat;             //длина обрезка
    bool used;              //найдена ли комбинация для этой заготовки?
    int* combination;       //указатель на массив, в котором будут перебираться комбинации
    int* buffer;            //указатель на массив, в котором будут храниться временные лучшие комбинации
    int* best_combination;  //указатель на массив, в котором будет храниться итоговая комбинация
};

struct Part
{
    int length; //длина детали
    bool used;  //использована ли она в итоговой комбинации?
};

//толщина пропила
int blade_thickness;

//количество деталей и досок
int length_parts = 1;
int length_boards = 0;

struct Node
{
    int data;
    struct Node* next;
};

struct Part* part = NULL;
struct Board* board = NULL;

//прочитать элемент связного списка
struct Node* n_read(struct Node* Hd, unsigned int id)
{
    struct Node* ptr = Hd;
    int i=0;
    for(; (i < id) && (ptr->next != NULL); i++)
    {
        ptr = ptr->next;
    }
    if(i == id){return ptr;}
    else{return NULL;}
}

//выделить память для нового элемента связного списка
struct Node* NewNode(int Data)
{
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->data=Data;
    node->next=NULL;
    return node;
}

//добавить значение в конец списка
void n_append(struct Node* Hd, int Data)
{
    struct Node* node_ptr=Hd;

    while(node_ptr->next != NULL)
    {
        node_ptr = node_ptr->next;
    }

    node_ptr->next = NewNode(Data);

}

//освободить память, выделенную под связный список
void n_free(struct Node* Hd)
{
    struct Node *now_ptr = Hd,
                *next_ptr = now_ptr->next;

        free(now_ptr);

    while(next_ptr != NULL)
    {
        now_ptr = next_ptr;
        next_ptr = now_ptr->next;

        free(now_ptr);
    }
}


#if DEBUG_MODE && DEBUG_MODE_ALL
//распечатать связный список
void n_print(struct Node* Hd)
{
    printf("\n");
    for(int i=0; n_read(Hd, i) != NULL; i++){
        printf("%d->", n_read(Hd, i)->data);
    }

    printf("NULL\n");
}

//распечатать состояние структуры "деталь"
void Pprint(struct Part x)
{
    printf("\tlength: %d\n\tused: %d\n", x.length, x.used);
}

//распечатать состояние структуры "заготовка"
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
//распечатать состояние всех расладок
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

/*вычичлить длинну, занимаемую всеми расположенными
на заготовке деталями с учётом ширины пропила*/
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

//вычислить расстояние от торца заготовки до конца n-й детали
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

/*проверка: можем ли мы расположить определённую
деталь на определенную заготовку?*/
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

//ращместить деталь на заготовке
void place_part(int idpart, int idboard)
{
    board[idboard].combination[board[idboard].counter] = idpart;
    board[idboard].counter++;
}

//убрать деталь с заготовки
void remove_last_part(int idboard)
{
    if(board[idboard].combination[0]!=0)
    {
        board[idboard].counter--;
        board[idboard].combination[board[idboard].counter]=0;
    }
}

//найти самый короткий обрезок на раскладке (в "буфере")
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

//очистить буфер и длиы обрезков
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

//скопировать перебираемую комбинацию в буфер
void copy_to_buffer(int idboard)
{
    board[idboard].remnat = board[idboard].length - summ_length_parts(idboard);
    board[idboard].best_counter = board[idboard].counter;

    for(int i=0; i<length_parts; i++)
    {
        board[idboard].buffer[i] = board[idboard].combination[i];
    }
}

/*проверить перебираемую комбинацию и скопировать её в буфер,
если она оптимальнее чем старая в буфере*/
void check(int idboard)
{
    int remnat = board[idboard].length - summ_length_parts(idboard);

    bool A = (remnat == board[idboard].remnat);
    bool E = A && (board[idboard].counter < board[idboard].best_counter);

    if(remnat < board[idboard].remnat)
    {
        copy_to_buffer(idboard);
    }
    else if(E)
    {
        copy_to_buffer(idboard);
    }
}

//все ли детали использованы?
bool all_parts_is_used()
{
    for(int i=0; i<length_parts; i++)
    {
        if(part[i].used == UNUSED)
            return FALSE;
    }
    return TRUE;
}

//начать перебор на одной заготовке
void recurs(int n, int idboard)
{
    for(int i=n; i<length_parts; i++)
    {
        if(can_place_part(i, idboard))
        {
            place_part(i, idboard);
            check(idboard);

            recurs(i+1, idboard);

            remove_last_part(idboard);
        }
    }
}

//скопировать комбинацию из буфера на итоговую раскладку
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

//безопасный ввод целых чисел
int fscan_uint(FILE* fp){

        start:
    while(TRUE){

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
            {
                goto start;
            }
        }
        return result;

    }
}

//элемент консольного интерфейса для ввода длин заготовок и деталей
void input(FILE* fp)
{
    int scanb;

    struct Node* boards = (struct Node*)calloc(sizeof(struct Node), 1);
    struct Node* parts = (struct Node*)calloc(sizeof(struct Node), 1);

    while(TRUE)
    {
        scanb = fscan_uint(fp);
        if(scanb == 0){break;}

        n_append(parts, scanb);
        length_parts++;
    }

    boards->data = fscan_uint(fp);
    boards->next = NULL;
    length_boards++;

    while(TRUE)
    {
        scanb = fscan_uint(fp);
        if(scanb==0){break;}

        n_append(boards, scanb);
        length_boards++;
    }
    //printf("\nТолщина пилы: ");

    blade_thickness = fscan_uint(fp);

#if DEBUG_MODE && DEBUG_MODE_ALL
n_print(parts);
n_print(boards);
#endif

    part = (struct Part*) malloc( sizeof(struct Part) * length_parts);

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

//сосчитать колличество цифр в цисле (для красивого отступа)
int digit_count(int number)
{
    int i;

    for(i=0; number != 0; i++)
    {
        number /= 10;
    }

    return i;
}

//распечатать итоговую комбинацию для всех заготовок
void print_combin()
{
    printf("Раскладка:\n");

    for(int i=0; i<length_boards; i++)
    {
        printf("\nЗаготовка №%d (%d):", i+1, board[i].length);

        for(int j=0; j<length_parts; j++)
        {
            if(board[i].best_combination[j]!=0)
                printf(" %d;", part[board[i].best_combination[j]].length);
        }

        printf("\n");

        for(int j=0; j<15+digit_count(i+1)+digit_count(board[i].length); j++){
            printf(" ");
        }

        for(int j=0; j<length_parts; j++)
        {
            if(board[i].best_combination[j]!=0)
                printf(" %d;", end_to_end_summ_length_parts(j, i));
        }

        printf("\n");
    }

    printf("\n\n");
    if(all_parts_is_used())
    {
        printf("Все детали распределены");
    }
    else
    {
        printf("Невместившиеся детали:");

        for(int i=0; i<length_parts; i++)
        {
            if(part[i].used == UNUSED)
                printf(" %d;", part[i].length);
        }
    }

    printf("\n\n");
}

//запустить процесс оптимизации
void optimize()
{
    for(int i=0; i<length_boards && ( all_parts_is_used() == FALSE); i++)
    {
        for(int j=0; j<length_boards; j++)
        {
            if(board[j].used == UNUSED)
                recurs(1, j);
        }
        //на план копируется комбинация с наименьшим обрезком,
        //паралельно отмечаются использованные детали
        copy_to_fin(shorter_remnat());

        //очистить буфер лучших комбинаций и сбросить длины обрезков
        clear();
    }//повторяем для остальных заготовок
}

//сохранить консольный вывод в текстовый файл
void save_print(FILE* fptr)
{
    fprintf(fptr, "Раскладка:\n");

    for(int i=0; i<length_boards; i++)
    {
        fprintf(fptr, "\nЗаготовка №%d (%d):", i+1, board[i].length);

        for(int j=0; j<length_parts; j++)
        {
            if(board[i].best_combination[j]!=0)
                fprintf(fptr, " %d;", part[board[i].best_combination[j]].length);
        }

        fprintf(fptr, "\n");

        for(int j=0; j<15+digit_count(i+1)+digit_count(board[i].length); j++){
            fprintf(fptr, " ");
        }

        for(int j=0; j<length_parts; j++)
        {
            if(board[i].best_combination[j]!=0)
                fprintf(fptr, " %d;", end_to_end_summ_length_parts(j, i));
        }

        fprintf(fptr, "\n");
    }

    fprintf(fptr, "\n\n");
    if(all_parts_is_used())
    {
        fprintf(fptr, "Все детали распределены");
    }
    else
    {
        fprintf(fptr, "Невместившиеся детали:");

        for(int i=0; i<length_parts; i++)
        {
            if(part[i].used == UNUSED)
            {
                fprintf(fptr, " %d;", part[i].length);
            }
        }
    }
        fprintf(fptr, "\n\n");
        printf("Раскладка сохранена\n");
}


int main()
{
    SetConsoleOutputCP(CP_UTF8); //установить кодировку utf-8 в консоли для вывода кирилицы

    FILE* finput = fopen("input.txt", "r");
    FILE* output = fopen("output.txt", "w+");

    if((finput != NULL) && (output != NULL))
    {
        /*при вводе деталей и заготовок, создаются массивы
        "объектов" деталей и заготовок, с которыми происходят дальнейшие действия*/
        input(finput);
        optimize();

        print_combin();
        save_print(output);

        fclose(finput);
        fclose(output);
    }
    else
    {
        if((finput == NULL) && (output == NULL)){
            printf("Не удалось открыть файлы input.txt и output.txt\n");
        } else if(finput == NULL){
            printf("Не удалось открыть файл input.txt\n");
        } else {
            printf("Не удалось открыть файл output.txt\n");
        }
    }

    system("pause");
    return 0;
}

