#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "defines/defines.c"
#include "structs/structs.c"
#include "input/input.h"
#include "alghoritm/alghoritm.h"
#include "output/output.h"

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
