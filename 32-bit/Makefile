32-bit: defines_structs.o input_output.o main.o alghoritm.o
	gcc -Wall -ggdb -o "cut-optimizer (32-bit).exe" defines_structs.o input_output.o main.o alghoritm.o

defines_structs.o:
	gcc -o defines_structs.o -c defines_structs.c

input_output.o:
	gcc -o input_output.o -c input_output.c

alghoritm.o:
	gcc -o alghoritm.o -c alghoritm.c

main.o:
	gcc -o main.o -c main.c

clean:
	rm defines_structs.o input_output.o main.o alghoritm.o
