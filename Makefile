C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c command-line/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h command-line/*.h)
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o cpu/switch_to32.o}

# Change this if your cross-compiler is somewhere else
CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
# -g: Use debugging symbols in gcc

# add "-D DEBUG" to command line arguments for debugging mode
# NOTE: when enableing or disabling command line mode, you will have to run `make clean` first
CFLAGS = -g -ffreestanding -Wall -Wextra -Wpedantic -Wno-unused-parameter -fno-exceptions -m32 -I$(dir $(realpath $(firstword $(MAKEFILE_LIST))))

KERNELSTART = 0x1000
# First rule is run by default
cosine.bin: boot/bootsect.bin kernel.bin
	cat $^ > cosine.bin

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext ${KERNELSTART} $^ --oformat binary

# Used for debugging purposes
kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext ${KERNELSTART} $^

run: cosine.bin
	qemu-system-i386 -drive file=cosine.bin,if=floppy,format=raw

# Open the connection to qemu and load our kernel-object file with symbols
debug: cosine.bin kernel.elf
	qemu-system-i386 -d int -s -fda cosine.bin -d guest_errors,int &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

new: clean cosine.bin

all: clean cosine.bin run

# Generic rules for wildcards
# To make an object, always compile from its .c
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o command-line/*.o
