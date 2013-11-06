OBJ=kernel_entry.o kernel_main.o monitor.o common.o
CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector

kernel: $(OBJ)
	ld -melf_i386 -o kernel.bin -Tlink.ld $^ --oformat binary

kernel_main.o: kernel_main.c
	gcc $(CFLAGS) -c $^ -o kernel_main.o 

common.o: common.c
	gcc $(CFLAGS) -c $^ -o common.o

monitor.o: monitor.c
	gcc $(CFLAGS) -c $^ -o monitor.o

kernel_entry.o: kernel_entry.asm
	nasm $^ -felf -o kernel_entry.o


