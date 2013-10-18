make:
	gcc -ffreestanding -c kernel.c -o kernel.o
	ld -o kernel.bin -Ttext 0x1000 kernel.o --oformat binary
	cat boot_sect.bin kernel.bin > os-image

clean:
	rm kernel.o kernel.bin os-image
