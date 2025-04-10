/**
 * Copyright (C) 2023-2025  Zhuravlev Artem
 *
 * This file is part of CutLinear-optimizer.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "defines_structs.h"
#include "alghoritm.h"

/*структура элемента связного списка*/
struct Node
{
        int data;
        struct Node* next;
};

/*хранит считываемый символ*/
int char_buffer = 0;

/*буферные связные списки для деталей и заготовок*/
struct Node* parts_list = NULL;
struct Node* boards_list = NULL;

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

/*ввод длин заготовок и деталей*/
void input(FILE* input_fp)
{
	int scanb;
	int d = 1;

	struct Node* boards = (struct Node*)calloc(sizeof(struct Node), 1);
	struct Node* parts = (struct Node*)calloc(sizeof(struct Node), 1);

	while(TRUE)
	{

		fscanf(input_fp, "%d", &scanb);
		if(scanb==0){break;}

		n_append(parts, scanb);
		length_parts++;
		d++;
	}


	d = 1;


	fscanf(input_fp, "%d", &boards->data);
	boards->next = NULL;
	length_boards++;
	d++;

	while(TRUE)
	{

		fscanf(input_fp, "%d", &scanb);
		if(scanb==0){break;}

		n_append(boards, scanb);
		length_boards++;
		d++;
	}

	fscanf(input_fp, "%d", &blade_thickness);

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

/*вычислить расстояние от торца заготовки до конца n-й детали*/
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

/*сосчитать колличество цифр в цисле (для красивого отступа)*/
int digit_count(int number)
{
        int i;
        for(i=0; number != 0; i++)
                number /= 10;

        return i;
}

/*вычичлить обрезок от заготовки на финальном плане*/
int remnat_final_plan(int idboard)
{
        int buffer = 0;
        for(int i = 0; i < length_parts; i++)
        {
                buffer += part[board[idboard].best_combination[i]].length;

                if( (i > 0) && (board[idboard].best_combination[i] != 0) )
                        buffer += blade_thickness;
        }

	if((board[idboard].length - buffer) <= blade_thickness)
	{
		return 0;
	}
	else
	{
		return (board[idboard].length - buffer);
	}
}

/*распечатать итоговую комбинацию для всех заготовок*/
void print_combin()
{
        printf("План раскроя:\n");

	int summ_length_remnats = 0;	/*для рассчета оптимальности*/
	int summ_length_counted_boards = 0;

        for(int i=0; i<length_boards; i++)
        {
		if(remnat_final_plan(i) < 1000)
		{
			summ_length_counted_boards += board[i].length;
			summ_length_remnats += remnat_final_plan(i);
		}

                printf("\nЗаготовка №%d (%d)\nДетали:   ", i+1, board[i].length);

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

		printf("\nОт начала:");

		for(int j=0; j<length_parts; j++){
			if(board[i].best_combination[j]!=0){
				printf(" %d;", end_to_end_summ_length_parts(j, i));
			}
		}

		printf("\nОбрезок: %d", remnat_final_plan(i));

		printf("\n");
        }

        printf("\n");
        if(all_parts_is_used())
                printf("Все детали распределены");
        else
        {
                printf("Невместившиеся детали:");

                for(int i=0; i<length_parts; i++)
                {
                        if(part[i].used == UNUSED)
                                printf(" %d;", part[i].length);
                }
        }

        printf("\nПроцент обрезков:\n%.2f\n", (100.0 * summ_length_remnats)/summ_length_counted_boards);
}

/*сохранить консольный вывод в текстовый файл*/
void save_print(FILE* fptr)
{
        fprintf(fptr, "План раскроя:\n");

	int summ_length_remnats = 0;	/*для рассчета оптимальности*/
	int summ_length_counted_boards = 0;

        for(int i=0; i<length_boards; i++)
        {
		if(remnat_final_plan(i) < 1000)
		{
			summ_length_counted_boards += board[i].length;
			summ_length_remnats += remnat_final_plan(i);
		}

                fprintf(fptr, "\nЗаготовка №%d (%d)\nДетали:   ", i+1, board[i].length);

                for(int j=0; j<length_parts; j++)
                {
                        int a = board[i].best_combination[j];
                        int c = end_to_end_summ_length_parts(j, i);

                        if(a!=0)
                        {
                                for(int k=0; k<digit_count(c)-digit_count(part[a].length); k++)
                                        fprintf(fptr, " ");

                                fprintf(fptr, " %d;", part[a].length);
                        }
                }

		fprintf(fptr, "\nОт начала:");

		for(int j=0; j<length_parts; j++){
			if(board[i].best_combination[j]!=0){
				fprintf(fptr, " %d;", end_to_end_summ_length_parts(j, i));
			}
		}

		fprintf(fptr, "\nОбрезок: %d", remnat_final_plan(i));

		fprintf(fptr, "\n");
        }

        fprintf(fptr, "\n");
        if(all_parts_is_used())
                fprintf(fptr, "Все детали распределены");
        else
        {
                fprintf(fptr, "Невместившиеся детали:");

                for(int i=0; i<length_parts; i++)
                {
                        if(part[i].used == UNUSED)
                                fprintf(fptr, " %d;", part[i].length);
                }
        }

        fprintf(fptr, "\nПроцент обрезков: %.2f\n", (100.0 * summ_length_remnats)/summ_length_counted_boards);
        printf("План сохраняется... \n");
}

