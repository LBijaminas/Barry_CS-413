image: boot kernel
	cat boot_sect.bin kernel.bin > os-image

kernel: kernel.c
	gcc -ffreestanding -c $^ -o kernel.o
	ld -o kernel.bin -Ttext 0x1000 kernel.o --oformat binary

boot: boot_sector.asm
	nasm $^ -f bin -o boot_sect.bin

clean:
	rm kernel.o kernel.bin os-image
