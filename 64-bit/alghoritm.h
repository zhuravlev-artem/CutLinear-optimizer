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
void Pprint(struct Part x);

/*распечатать состояние структуры "заготовка"*/
void Bprint(struct Board x);
#endif

#if DEBUG_MODE_ALL
/*распечатать состояние всех расладок*/
void Aprint();
#endif

/*вычичлить длинну, занимаемую всеми расположенными
на заготовке деталями с учётом ширины пропила*/
int summ_length_parts(int idboard);

/*проверка: можем ли мы расположить определённую
деталь на определенную заготовку?*/
bool can_place_part(int idpart, int idboard);

/*ращместить деталь на заготовке*/
void place_part(int idpart, int idboard);

/*убрать деталь с заготовки*/
void remove_last_part(int idboard);

/*найти самый короткий обрезок на раскладке (в "буфере")*/
int shorter_remnat();

/*очистить буфер и длиы обрезков*/
void clear();

/*скопировать перебираемую комбинацию в буфер*/
void copy_to_buffer(int idboard);

/*проверить перебираемую комбинацию и скопировать её в буфер,
если она оптимальнее чем старая в буфере*/
void check(int idboard);

/*все ли детали использованы?*/
bool all_parts_is_used();

/*начать перебор на одной заготовке*/
void recurs(int n, int idboard);

/*скопировать комбинацию из буфера на итоговую раскладку*/
void copy_to_fin(int idboard);

/*запустить процесс оптимизации*/
void optimize();


#endif //ALGHORITM
