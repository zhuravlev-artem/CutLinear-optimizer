x64: defines_structs.o input.o alghoritm.o main.o
	gcc -Wall -o Cut-optimiser defines_structs.o input.o alghoritm.o main.o

defines_structs.o:
	gcc -o defines_structs.o -c defines_structs.c

input.o:
	gcc -o input.o -c input.c

alghoritm.o:
	gcc -o alghoritm.o -c alghoritm.c

main.o:
	gcc -o main.o -c main.c

clean:
	rm defines_structs.o input.o alghoritm.o main.o
