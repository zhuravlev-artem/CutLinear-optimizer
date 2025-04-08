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

#ifndef ALGHORITM
#define ALGHORITM

#if DEBUG_MODE
/*распечатать состояние структуры "деталь"*/
void Pprint(struct Part x)
{
        printf("\tlength: %d\n\tused: %d\n", x.length, x.used);
}

/*распечатать состояние структуры "заготовка"*/
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
/*распечатать состояние всех расладок*/
void Aprint()
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

        printf("\nбуфер:\n");

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

        printf("\nитог:\n");

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
                        printf("Неиспользована");
                else
                        printf("использована");

                printf("\n");
        }

        printf("\nиспользуются:\t");

        for(int i=1; i<length_parts; i++)
                printf("%d:%d; ", part[i].length, part[i].used);

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
                        buffer += blade_thickness;
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
                buffer = part[idpart].length;
        else
                buffer = part[idpart].length + summ + blade_thickness;

        if(buffer <= board[idboard].length)
                return TRUE;
        else
                return FALSE;
}

/*ращместить деталь на заготовке*/
void place_part(int idpart, int idboard)
{
        board[idboard].combination[board[idboard].counter] = idpart;
        board[idboard].counter++;
}

/*убрать деталь с заготовки*/
void remove_last_part(int idboard)
{
        if(board[idboard].combination[0]!=0)
        {
                board[idboard].counter--;
                board[idboard].combination[board[idboard].counter]=0;
        }
}

/*найти самый короткий обрезок на раскладке (в "буфере")*/
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

/*очистить буфер и длиы обрезков*/
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

/*скопировать перебираемую комбинацию в буфер*/
void copy_to_buffer(int idboard)
{
        board[idboard].remnat = board[idboard].length - summ_length_parts(idboard);
        board[idboard].best_counter = board[idboard].counter;

        for(int i=0; i<length_parts; i++)
                board[idboard].buffer[i] = board[idboard].combination[i];
}

/*проверить перебираемую комбинацию и скопировать её в буфер,
если она оптимальнее чем старая в буфере*/
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

/*все ли детали использованы?*/
bool all_parts_is_used()
{
        for(int i=0; i<length_parts; i++)
                if(part[i].used == UNUSED) return FALSE;

        return TRUE;
}

/*начать перебор на одной заготовке*/
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

/*скопировать комбинацию из буфера на итоговую раскладку*/
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



#endif //ALGHORITM
