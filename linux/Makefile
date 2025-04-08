all: linux_64-bit linux_32-bit

linux_32-bit: defines_structs_32-bit.o input_output_32-bit.o main_32-bit.o alghoritm_32-bit.o
	gcc -Wall -ggdb -m32 -o "../cutlinear-optimizer_32-bit" "defines_structs_32-bit.o" "input_output_32-bit.o" "main_32-bit.o" "alghoritm_32-bit.o"

defines_structs_32-bit.o:
	gcc -m32 -o "defines_structs_32-bit.o" -c defines_structs.c

input_output_32-bit.o:
	gcc -m32 -o "input_output_32-bit.o" -c input_output.c

alghoritm_32-bit.o:
	gcc -m32 -o "alghoritm_32-bit.o" -c alghoritm.c

main_32-bit.o:
	gcc -m32 -o "main_32-bit.o" -c main.c

linux_64-bit: defines_structs.o input_output.o main.o alghoritm.o
	gcc -Wall -ggdb -o ../cutlinear-optimizer defines_structs.o input_output.o main.o alghoritm.o

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
	rm "defines_structs_32-bit.o" "input_output_32-bit.o" "main_32-bit.o" "alghoritm_32-bit.o"
