OBJ=kernel_entry.o kernel.o
CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector

kernel: $(OBJ)
	ld -melf_i386 -o kernel.bin -Ttext 0x1000 $^ --oformat binary

kernel.o: kernel.c
	gcc $(CFLAGS) -c $^ -o kernel.o

kernel_entry.o: kernel_entry.asm
	nasm $^ -f elf -o kernel_entry.o


