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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef DEFINES_STRUCTS
#define DEFINES_STRUCTS

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

/*количество деталей и досок*/
int length_parts = 1;
int length_boards = 0;

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

/*толщина пропила*/
int blade_thickness = 0;

struct Part* part = NULL;
struct Board* board = NULL;


#endif //DEFINES_STRUCTS
