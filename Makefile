# I know this is not The Perfect Makefile, but it does the job. 
# Sure, there is a lot of space for further improvements. 

CC=i686-elf-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra
LD=i686-elf-gcc
AS=i686-elf-as
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc

pulgaOs: pulgaOs.bin

pulgaOs.bin: boot.o kernel.o init.o head.o io.o keyboard.o
	$(LD) -T linker.ld -o output/boot/image.bin output/boot.o output/kernel.o output/head.o output/init.o output/io.o output/keyboard.o $(LDFLAGS)

boot.o: 
	$(AS) boot/boot.S -o output/boot.o

kernel.o: kernel.c 
	$(CC) -c kernel.c -o output/kernel.o $(CFLAGS)

init.o:
	$(CC) -c boot/init.c -o output/init.o $(CFLAGS)

head.o: 
	$(AS) boot/head.S -o output/head.o

io.o:
	$(AS) boot/io.S -o output/io.o

keyboard.o:
	$(AS) boot/keyboard.S -o output/keyboard.o
