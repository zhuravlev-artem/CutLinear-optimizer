CC = gcc

all: linux_64-bit linux_32-bit so-linux-64

so-linux-64:
	$(CC) -ggdb -fPIC -o defines_structs_so.o -c defines_structs.c
	$(CC) -ggdb -fPIC -o input_output_so.o -c input_output.c
	$(CC) -ggdb -fPIC -o py_interface_so.o -c py_interface.c
	$(CC) -ggdb -fPIC -o alghoritm_so.o -c alghoritm.c

	$(CC) -ggdb -shared -fPIC -Wall -o libcutlinear-optimizer.so defines_structs_so.o input_output_so.o py_interface_so.o alghoritm_so.o



linux_32-bit:
	$(CC) -ggdb -m32 -o "defines_structs_32-bit.o" -c defines_structs.c
	$(CC) -ggdb -m32 -o "input_output_32-bit.o" -c input_output.c
	$(CC) -ggdb -m32 -o "alghoritm_32-bit.o" -c alghoritm.c
	$(CC) -ggdb -m32 -o "main_32-bit.o" -c main.c

	$(CC) -ggdb -Wall -m32 -o "../cutlinear-optimizer_32-bit" "defines_structs_32-bit.o" "input_output_32-bit.o" "main_32-bit.o" "alghoritm_32-bit.o"



linux_64-bit:
	$(CC) -ggdb -o defines_structs.o -c defines_structs.c
	$(CC) -ggdb -o input_output.o -c input_output.c
	$(CC) -ggdb -o alghoritm.o -c alghoritm.c
	$(CC) -ggdb -o main.o -c main.c

	$(CC) -ggdb -Wall -o ../cutlinear-optimizer defines_structs.o input_output.o main.o alghoritm.o


clean:
	rm defines_structs_so.o input_output_so.o py_interface_so.o alghoritm_so.o 
	rm defines_structs.o input_output.o main.o alghoritm.o
	rm "defines_structs_32-bit.o" "input_output_32-bit.o" "main_32-bit.o" "alghoritm_32-bit.o"
