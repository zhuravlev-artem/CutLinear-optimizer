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
                input(finput);
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

