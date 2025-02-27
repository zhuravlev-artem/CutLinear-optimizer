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
#include <windows.h>

int main()
{
        FILE* finput = fopen("input.txt", "r");
        FILE* output = fopen("output.txt", "w+");

        if((finput != NULL) && (output != NULL))
        {
                /*при вводе деталей и заготовок, создаются
                массивы "экземпляров" деталей и заготовок,
                с которыми дальше работает программа*/
		printf("Reading input...\n");
                input(finput);
		printf("Optimizing...\n");
                optimize();

                print_combin();
                save_print(output);

                fclose(finput);
                fclose(output);
		printf("Plan was saved\n");
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

