# $^ - all required files
# $@ - output files
# $< - first required file

C_SOURCES=$(wildcard *.c ../drivers/*.c)
HEADERS=$(wildcard *.h ../*.h)

# Convert the *. c filenames to *. o to give a list of object files to build
OBJ=${C_SOURCES:.c=.o}

# Defaul build target
all: os-image

# Run bochs to simulate booting of our code .
run: all

os-image: asm/boot_sector.bin kernel.bin
	cat $^ > os-image

#kernel.bin: kernel_entry.o gdt.o interrupt.o ${OBJ}
	#ld -melf_i386 -o $@ -Tlink.ld $^ --oformat binary

kernel.bin: kernel_entry.o isr.o descriptor_tables.o gdt.o interrupt.o timer.o common.o monitor.o paging.o main.o
	ld -melf_i386 -o $@ -Tlink.ld -Ttext 0x1000 $^ --oformat binary

%.o: %.c ${HEADERS}
	gcc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -ffreestanding -c $< -o $@
# Assemble the kernel_entry .
%.o: %.asm
	nasm $< -f elf32 -o $@
%.bin: ../asm/%.asm
	nasm $< -f bin -I '../asm/' -o $@
clean:
	rm -fr *.bin *.dis *.o os-image
	rm -fr Lukas_kernel/*.o Lukas_kernel/boot/*.bin drivers/*.o
