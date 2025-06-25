CC = gcc

all: linux_64-bit linux_32-bit so-linux-64

so-linux-64:
	$(CC) -fPIC -o defines_structs_so.o -c defines_structs.c
	$(CC) -fPIC -o input_output_so.o -c input_output.c
	$(CC) -fPIC -o py_interface_so.o -c py_interface.c
	$(CC) -fPIC -o alghoritm_so.o -c alghoritm.c

	$(CC) -shared -fPIC -Wall -ggdb -o libcutlinear-optimizer.so defines_structs_so.o input_output_so.o py_interface_so.o alghoritm_so.o



linux_32-bit:
	$(CC) -m32 -o "defines_structs_32-bit.o" -c defines_structs.c
	$(CC) -m32 -o "input_output_32-bit.o" -c input_output.c
	$(CC) -m32 -o "alghoritm_32-bit.o" -c alghoritm.c
	$(CC) -m32 -o "main_32-bit.o" -c main.c

	$(CC) -Wall -ggdb -m32 -o "../cutlinear-optimizer_32-bit" "defines_structs_32-bit.o" "input_output_32-bit.o" "main_32-bit.o" "alghoritm_32-bit.o"



linux_64-bit:
	$(CC) -o defines_structs.o -c defines_structs.c
	$(CC) -o input_output.o -c input_output.c
	$(CC) -o alghoritm.o -c alghoritm.c
	$(CC) -o main.o -c main.c

	$(CC) -Wall -ggdb -o ../cutlinear-optimizer defines_structs.o input_output.o main.o alghoritm.o


clean:
	rm defines_structs_so.o input_output_so.o py_interface_so.o alghoritm_so.o 
	rm defines_structs.o input_output.o main.o alghoritm.o
	rm "defines_structs_32-bit.o" "input_output_32-bit.o" "main_32-bit.o" "alghoritm_32-bit.o"
