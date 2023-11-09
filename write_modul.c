
{
    FILE* fptr = fopen("output.xls", "w");
    if(fptr != NULL)
    {
        fprintf(fptr, "№№ заготовок\t№№ деталей\n");

        for(int i=0; i<length_boards; i++)
        {
            fprintf(fptr, "%d\t", i+1);
            for(int j=0; j<length_parts; j++)
            {
                fprintf(fptr, "№ %d (%d)\t", board[i].best_combination[j], part[board[i].best_combination[j]].length);
            }
            fprintf(fptr, "\n");
        }
        fclose(fptr);
        printf("File has been written\n");
    }
}
