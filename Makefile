# $^ - all required files
# $@ - output files
# $< - first required file

C_SOURCES=$(wildcard kernel/*.c drivers/*.c)
HEADERS=$(wildcard kernel/*.h drivers/*.h)

# Convert the *. c filenames to *. o to give a list of object files to build
OBJ=${C_SOURCES:.c=.o}

# Defaul build target
all: os-image

# Run bochs to simulate booting of our code .
run: all
	bochs


os-image: boot/boot_sect.bin kernel.bin
	cat $^ > os-image

kernel.bin: kernel/kernel_entry.o ${OBJ}
	ld -o $@ - Ttext 0x1000 $^ --oformat binary

%.o: %.c ${HEADERS}
	gcc -ffreestanding -c $< -o $@

# Assemble the kernel_entry .
%.o: %.asm
	nasm $< -f elf -o $@
%.bin: %.asm
	nasm $< -f bin -I '../../16 bit / ' -o $@
clean:
	rm -fr *.bin *.dis *.o os-image
	rm -fr kernel/*.o boot/*.bin drivers/*.o
