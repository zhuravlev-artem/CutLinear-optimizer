CC = gcc

all: linux_64-bit linux_32-bit

so-linux-64: defines_structs_so.o input_output_so.o py_interface_so.o alghoritm_so.o
	$(CC) -shared -fPIC -Wall -ggdb -o libcutlinear-optimizer.so defines_structs_so.o input_output_so.o py_interface_so.o alghoritm_so.o

defines_structs_so.o:
	$(CC) -fPIC -o defines_structs_so.o -c defines_structs.c

input_output_so.o:
	$(CC) -fPIC -o input_output_so.o -c input_output.c

py_interface_so.o:
	$(CC) -fPIC -o py_interface_so.o -c py_interface.c

alghoritm_so.o:
	$(CC) -fPIC -o alghoritm_so.o -c alghoritm.c



linux_32-bit: defines_structs_32-bit.o input_output_32-bit.o main_32-bit.o alghoritm_32-bit.o
	$(CC) -Wall -ggdb -m32 -o "../cutlinear-optimizer_32-bit" "defines_structs_32-bit.o" "input_output_32-bit.o" "main_32-bit.o" "alghoritm_32-bit.o"

defines_structs_32-bit.o:
	$(CC) -m32 -o "defines_structs_32-bit.o" -c defines_structs.c

input_output_32-bit.o:
	$(CC) -m32 -o "input_output_32-bit.o" -c input_output.c

alghoritm_32-bit.o:
	$(CC) -m32 -o "alghoritm_32-bit.o" -c alghoritm.c

main_32-bit.o:
	$(CC) -m32 -o "main_32-bit.o" -c main.c



linux_64-bit: defines_structs.o input_output.o main.o alghoritm.o
	$(CC) -Wall -ggdb -o ../cutlinear-optimizer defines_structs.o input_output.o main.o alghoritm.o

defines_structs.o:
	$(CC) -o defines_structs.o -c defines_structs.c

input_output.o:
	$(CC) -o input_output.o -c input_output.c

alghoritm.o:
	$(CC) -o alghoritm.o -c alghoritm.c

main.o:
	$(CC) -o main.o -c main.c

clean:
	rm defines_structs.o input_output.o main.o alghoritm.o
	rm "defines_structs_32-bit.o" "input_output_32-bit.o" "main_32-bit.o" "alghoritm_32-bit.o"
