#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "defines_structs.h"
#include "input.c"
#include "alghoritm.c"

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

/*запустить процесс оптимизации*/
void optimize()
{
        for(int i=0; i<length_boards && !all_parts_is_used(); i++)
        {
                for(int j=0; j<length_boards; j++)
                {
                        if(board[j].used == UNUSED)
                                recurs(1, j);
                }
                /*на план копируется комбинация с наименьшим обрезком,*/
                /*паралельно отмечаются использованные детали*/
                copy_to_fin(shorter_remnat());

                /*очистить буфер лучших комбинаций и сбросить длины обрезков*/
                clear();
        }/*повторяем для остальных заготовок*/
}

/*распечатать итоговую комбинацию для всех заготовок*/
void print_combin()
{
        printf("План раскроя:\n");

        for(int i=0; i<length_boards; i++)
        {
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
        printf("\n\n");
}

/*сохранить консольный вывод в текстовый файл*/
void save_print(FILE* fptr)
{
    fprintf(fptr, "План раскроя:\n");

        for(int i=0; i<length_boards; i++){
                fprintf(fptr, "\nЗаготовка №%d (%d)\nДетали:   ", i+1, board[i].length);

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

                fprintf(fptr, "\nОт начала:");

                for(int j=0; j<length_parts; j++){
                        if(board[i].best_combination[j]!=0){
                                fprintf(fptr, " %d;", end_to_end_summ_length_parts(j, i));
                        }
                }

                fprintf(fptr, "\n");
        }

        fprintf(fptr, "\n");
        if(all_parts_is_used()){
                fprintf(fptr, "Все детали распределены");
        }else{
                fprintf(fptr, "Невместившиеся детали:");

                for(int i=0; i<length_parts; i++){
                        if(part[i].used == UNUSED){
                                fprintf(fptr, " %d;", part[i].length);
                        }
                }
        }
        fprintf(fptr, "\n\n");
        printf("План сохраняется... \n");
}


int main()
{
        FILE* finput = fopen("input.txt", "r");
        FILE* output = fopen("output.txt", "w+");

        if((finput != NULL) && (output != NULL))
        {
                /*при вводе деталей и заготовок, создаются
                массивы "экземпляров" деталей и заготовок,
                с которыми дальше работает программа*/
                input(finput);
                optimize();

                print_combin();
                save_print(output);

                fclose(finput);
                fclose(output);
                printf("План сохранен\n");
        }
        else
        {
                if((finput == NULL) && (output == NULL))
                        printf("Не удалось открыть файлы input.txt и output.txt\n");
                else if(finput == NULL)
                        printf("Не удалось открыть файл input.txt\n");
                else
                        printf("Не удалось открыть файл output.txt\n");
        }

        printf("Press any key to continue . . . ");
        getchar();
        return 0;
}

