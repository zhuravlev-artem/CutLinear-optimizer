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

/*сосчитать колличество цифр в цисле (для красивого отступа)*/
int digit_count(int number)
{
        int i;
        for(i=0; number != 0; i++)
                number /= 10;

        return i;
}


