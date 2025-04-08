#include <stdio.h>

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
	char* inp_filename = "";
	char* prefics = "cut_plan - ";

	if(argc == 1)
	{
		inp_filename = "input.txt";
		/*printf("аргумент не передан\n");*/
	}
	else
	{
		inp_filename = arg[1];
	}

	char out_filename[charcount(inp_filename) + charcount(prefics)];
	out_filename[0] = '\0';
	strmerge(prefics, inp_filename, out_filename);

	printf("%s\n", inp_filename);
	printf("%s\n", out_filename);
	
	return 0;

}
