32-bit: defines_structs.o input_output.o main.o alghoritm.o
	gcc -Wall -ggdb -m32 -o "..\cutlinear-optimizer (32-bit).exe" defines_structs.o input_output.o main.o alghoritm.o

defines_structs.o:
	gcc -m32 -o defines_structs.o -c defines_structs.c

input_output.o:
	gcc -m32 -o input_output.o -c input_output.c

alghoritm.o:
	gcc -m32 -o alghoritm.o -c alghoritm.c

main.o:
	gcc -m32 -o main.o -c main.c

clean:
	del defines_structs.o input_output.o main.o alghoritm.o
