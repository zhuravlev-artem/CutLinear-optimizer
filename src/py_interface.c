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
#include "input_output.h"
#include "alghoritm.h"

int start_optimize(char* input_file, char* output_file)
{
        FILE* finput = fopen(input_file, "r");
        FILE* foutput = fopen(output_file, "w+");

	/*fseek(finput, 0, seek_set);
	fseek(foutput, 0, SEEK_SET);*/

        if((finput != NULL) && (foutput != NULL))
        {
                /*при вводе деталей и заготовок, создаются
                массивы "экземпляров" деталей и заготовок,
                с которыми дальше работает программа*/
		printf("Чтение ввода...\n");
                input(finput);
		printf("Оптимизация...\n");
                optimize();

                print_combin();
                save_print(foutput);

		free_structs();

                fclose(finput);
                fclose(foutput);
                printf("План сохранен\n");
        }
        else
        {
		//3 - не удалось открыть файлы input_file, output_file
		//1 - не удалось открыть файл input_file
		//2 - не удалось открыть файл output_file

                if((finput == NULL) && (foutput == NULL))
			return 3;
                else if(finput == NULL)
			return 1;
                else
			return 2;
        }

	return 0;
}
