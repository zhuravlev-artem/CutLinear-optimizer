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

#ifdef _WIN32
#include <windows.h>
#endif

int charcount(char* str)
{
	int i;
	for(i = 0; str[i] != '\0'; i++)
	{}
	return i;
}

void strmerge(char* str1, char* str2, char* str_result)
{
	int i = 0;
	for(; str1[i] != '\0'; i++)
	{
		str_result[i] = str1[i];
	}
	for(int k=0; str2[k] != '\0'; k++, i++)
	{
		str_result[i] = str2[k];
	}
	str_result[i] = '\0';
}

int main(int argc, char** arg)
{
#ifdef _WIN64
        SetConsoleOutputCP(CP_UTF8);
#endif
	char* inp_filename = "";
	char* prefics = "cut_plan - ";

	/*если аргумент передан, то использовать его в имени выходного файла. Иначе - сделать вид, что исходный файл - input.txt*/
	if(argc == 1) 
	{
		inp_filename = "input.txt";
		/*printf("аргумент не передан\n");*/
	}
	else
	{
		inp_filename = arg[1];
	}

	char out_filename[charcount(inp_filename) + charcount(prefics)]; /*создать массив символов размером, достаточным для объединения префикса и названия файла*/
	out_filename[0] = '\0';

	strmerge(prefics, inp_filename, out_filename);
	
        FILE* finput = fopen(inp_filename, "r");
        FILE* foutput = fopen(out_filename, "w+");

        if((finput != NULL) && (foutput != NULL))
        {
                /*при вводе деталей и заготовок, создаются
                массивы "экземпляров" деталей и заготовок,
                с которыми дальше работает программа*/
#if defined _WIN32 && defined __i386__
		printf("Reading input...\n");
#elif
		printf("Чтение ввода...\n");
#endif
                input(finput);
#if defined _WIN32 && defined __i386__
		printf("Optimizing...\n");
#elif
		printf("Оптимизация...\n");
#endif
                optimize();

                print_combin();
                save_print(foutput);

                fclose(finput);
                fclose(foutput);
#if defined _WIN32 && defined __i386__
                printf("Plan saved\n");
#elif
                printf("План сохранен\n");
#endif
        }
        else
        {
                if((finput == NULL) && (foutput == NULL))
#if defined _WIN32 && defined __i386__
                        printf("fail to open %s and %s\n", inp_filename, out_filename);
                else if(finput == NULL)
                        printf("fail to open %s\n", inp_filename);
                else
                        printf("fail to open %s\n", out_filename);
#elif
                        printf("Не удалось открыть файлы %s и %s\n", inp_filename, out_filename);
                else if(finput == NULL)
                        printf("Не удалось открыть файл %s\n", inp_filename);
                else
                        printf("Не удалось открыть файл %s\n", out_filename);
#endif
        }

        printf("Press ENTER to continue . . . ");
        getchar();
        return 0;
}

