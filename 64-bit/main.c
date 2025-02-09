#include "defines_structs.h"
#include "input_output.h"
#include "alghoritm.h"
#include <windows.h>

int main()
{
        SetConsoleOutputCP(CP_UTF8);
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

	system("pause");
        return 0;
}

