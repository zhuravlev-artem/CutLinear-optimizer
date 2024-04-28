#define M_PARTS 1
#define M_BOARDS 2
#define M_BLADE 3
#define M_NOP 4

#define SM_IN 1
#define SM_OUT 2
#define SM_LETER 3

#define ERR_GOOD 1
#define ERR_LETER 2
#define ERR_SLN 3
#define ERR_NULL 4
#define ERR_UNKNOW 5
#define ERR_EOF 6

int char_buffer = 0;


struct Part* part = NULL;
struct Board* board = NULL;
struct Node* parts_list = NULL;
struct Node* boards_list = NULL;



bool is_leter(int c){
        return (!(c >= '0' && c <= '9') && !(c == ' ' || c == '\t'));
}

bool is_number(int c){
        return (c >= '0' && c <= '9');
}

bool is_space(int c){
        return (c == ' ' || c == '\t');
}

void addls(int* mode, int len, int cnt)
{
        int* counter = NULL;
        struct Node* Hd_node = NULL;

        if(*mode == M_PARTS)
        {
                counter = &length_parts;
                Hd_node = parts_list;

                if(len == 0 && cnt == -1) *mode = M_BOARDS;
        }
        else if(*mode == M_BOARDS)
        {
                counter = &length_boards;
                Hd_node = boards_list;

                if(len == 0 && cnt == -1) *mode = M_BLADE;
        }
        else if(*mode == M_BLADE)
        {
                blade_thickness = len;
                *mode = M_NOP;
                return;
        }

        if(len == -1) len = 1;

        for(int i=0; i<cnt; i++)
        {
                n_append(Hd_node, len);
                (*counter)++;
        }
}

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
                else if(is_leter(char_buffer))
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
                                *err = ERR_LETER;
                                return;
                        }
                }
        }
        *err = ERR_UNKNOW;
        return;
}

void skip(FILE* fp, int* err)
{
        while(TRUE)
        {
                char_buffer = fgetc(fp);
                if(char_buffer == '\n')
                {
                        *err = 0;
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
                else if(err == ERR_LETER) goto start;
                else if(err == ERR_NULL) goto start;
                else if(err == ERR_EOF) break;

                if(err != ERR_SLN){
                        getnumber(fp, &count, &err);

                        if(err == ERR_LETER) goto start;
                        else if(err == ERR_EOF) break;

                        if(err != ERR_SLN)
                        {
                                skip(fp, &err);
                                if (err == ERR_EOF) break;
                        }
                }

                addls(&mode, length, count);/////////////////////////////
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

        n_print(parts_list);
        printf("\n%d\n", length_parts);
        n_print(boards_list);
        printf("\n%d\n", length_boards);
        printf("\n%d\n", blade_thickness);

        n_free(parts_list);
        n_free(boards_list);



}

int main()
{
        FILE* fp = fopen("input.txt", "r");
        input(fp);


        return 0;
}
