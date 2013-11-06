# $^ - all required files
# $@ - output files
# $< - first required file

C_SOURCES=$(wildcard Lukas_kernel/*.c drivers/*.c)
HEADERS=$(wildcard Lukas_kernel/*.h drivers/*.h)

# Convert the *. c filenames to *. o to give a list of object files to build
OBJ=${C_SOURCES:.c=.o}

# Defaul build target
all: os-image

# Run bochs to simulate booting of our code .
run: all
	bochs


os-image: boot_sector.bin kernel.bin
	cat boot_sector.bin kernel.bin > os-image

kernel.bin: Lukas_kernel/kernel_entry.o Lukas_kernel/gdt.o Lukas_kernel/interrupt.o ${OBJ}
	ld -melf_i386 -o $@ -Tlink.ld $^ --oformat binary

%.o: Lukas_kernel/%.c ${HEADERS}
	gcc -m32 -fstack-protector-all -nostdinc -nodefaultlibs -nostdlib -fno-builtin -ffreestanding -c $< -o $@

# Assemble the kernel_entry .
Lukas_kernel/%.o: Lukas_kernel/%.asm
	nasm $< -f elf -o $@
%.bin: asm/%.asm
	nasm $< -f bin -I 'asm/' -o $@
clean:
	rm -fr *.bin *.dis *.o os-image
	rm -fr Lukas_kernel/*.o Lukas_kernel/boot/*.bin drivers/*.o
