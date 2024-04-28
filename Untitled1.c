#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

#define TRUE 1
#define FALSE 0

#define M_LENGTH 1
#define M_COUNT 2
#define M_SKIP 3

#define SM_IN 1
#define SM_OUT 2
#define SM_LETER 3

#define ERR_GOOD 1
#define ERR_LETER 2
#define ERR_SLN 3
#define ERR_NULL 4
#define ERR_UNKNOW 5
#define ERR_EOF 6

int ch = 0;

bool leter(int c)
{
        return (!(c >= '0' && c <= '9') && !(c == ' ' || c == '\t'));
}

bool number(int c)
{
        return (c >= '0' && c <= '9');
}

bool space(int c)
{
        return (c == ' ' || c == '\t');
}

void printls(int len, int count)
{
        for(int i=0; i<count; i++)
        {
                printf("%d\n", len);
        }
}
/**
*повтор функции
*/
int getlength(FILE* fp, int* len)
{
        *len = 0;
        int length = 0;
        int sub_mode = SM_OUT;
        while(TRUE)
        {
                ch = fgetc(fp);
                if(space(ch))
                {
                        if(sub_mode == SM_IN)
                        {
                                *len = length;
                                return ERR_GOOD;
                        }
                }
                else if(number(ch))
                {
                        if(sub_mode == SM_OUT)
                        {
                                sub_mode = SM_IN;
                        }
                        length *= 10;
                        length += ch -'0';
                }
                else if(leter(ch))
                {
                        if(ch == '\n')
                        {
                                if(sub_mode == SM_IN)
                                {
                                        *len = length;
                                        return ERR_SLN;
                                }
                                else if(sub_mode == SM_OUT)
                                {
                                        return ERR_NULL;
                                }
                        }
                        if(ch == EOF)
                        {
                                *len = length;
                                return ERR_EOF;
                        }
                        else
                        {
                                return ERR_LETER;
                        }
                }
        }
        return ERR_UNKNOW;
}

int getcount(FILE* fp, int* cnt)
{
        *cnt = 0;
        int count = 0;
        int sub_mode = SM_OUT;
        while(TRUE)
        {
                ch = fgetc(fp);
                if(space(ch))
                {
                        if(sub_mode == SM_IN)
                        {
                                *cnt = count;
                                return ERR_GOOD;
                        }
                }
                else if(number(ch))
                {
                        if(sub_mode == SM_OUT)
                        {
                                sub_mode = SM_IN;
                        }
                        count *= 10;
                        count += ch - '0';
                }
                else if(leter(ch))
                {
                        if(ch == '\n')
                        {
                                *cnt = count;
                                return ERR_SLN;
                        }
                        if(ch == EOF)
                        {
                                *cnt = count;
                                return ERR_EOF;
                        }
                        else
                        {
                                return ERR_LETER;
                        }
                }
        }
        return ERR_UNKNOW;
}

int skip(FILE* fp)
{
        while(TRUE)
        {
                ch = fgetc(fp);
                if(ch == '\n')
                {
                        return 0;
                }
                else if(ch == EOF)
                {
                        return ERR_EOF;
                }
        }
}

int main()
{
        FILE* fp = fopen("input.txt", "r");

        int length;
        int count;
        int err;

        while(TRUE)
        {
                start:

                length = 0;
                count = 0;
                err = 0;



                err = getlength(fp, &length);
                if(err == ERR_SLN){
                        printls(length, 1);
                        goto start;
                }
                else if(err == ERR_LETER) goto start;
                else if(err == ERR_NULL) goto start;
                else if(err == ERR_EOF) break;



                err = getcount(fp, &count);
                if(err == ERR_SLN){
                        printls(length, count);
                        goto start;
                }
                else if(err == ERR_LETER) goto start;
                else if(err == ERR_EOF) break;



                err = skip(fp);
                if (err == ERR_EOF)
                        break;

                printls(length, count);
        }
        //system("pause");
        return 0;
}
