FILE* fptr = fopen("output.xls", "w+");
    if(fptr != NULL)
    {
        fprintf(fptr, "# boards\t# parts\n");

        for(int i=0; i<length_boards; i++)
        {
            fprintf(fptr, "board %d:\t", i+1);
            for(int j=0; j<length_parts; j++)
            {
                if(board[i].best_combination[j] != 0)
                    fprintf(fptr, "#%d (%d)\t", board[i].best_combination[j], part[board[i].best_combination[j]].length);
            }
            fprintf(fptr, "\n");
        }
        fprintf(fptr, "\nUnused parts:\t");
        for(int i=0; i<length_parts; i++){
            if(part[i].used == UNUSED){
                fprintf(fptr, "#%d (%d)", i+1, part[i].length);
            }
        }
        fclose(fptr);
        printf("File has been written\n");
    }
