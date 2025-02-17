CROSS_COMPILE = x86_64-elf-
CC = $(CROSS_COMPILE)gcc
LD = ld
AS = nasm
CFLAGS = -ffreestanding -mno-red-zone -m64 -nostdlib -Wall -Wextra

all: iso/build/kernel.elf iso

# Assemble entry.S
entry.o: src/boot/entry.asm
	$(AS) -f elf64 src/boot/entry.asm -o entry.o

# Compile kernel.c
kernel.o: src/kernel/kernel.c
	$(CC) $(CFLAGS) -c src/kernel/kernel.c -o kernel.o

# Assemble header.S
header.o: src/boot/header.asm
	$(AS) -f elf64 src/boot/header.asm -o header.o

# Modify the linking step to include header.o
iso/build/kernel.elf: header.o entry.o kernel.o
	mkdir -p iso/build
	$(LD) -n -T src/boot/linker.ld -o iso/build/kernel.elf header.o entry.o kernel.o

# Generate ISO with GRUB
iso: iso/build/kernel.elf
	mkdir -p iso/boot/grub
	ln -sf ../build/kernel.elf iso/boot/kernel.elf  # Symlink kernel
	# cp src/boot/grub.cfg iso/boot/grub/  # Copy GRUB config
	grub2-mkrescue -o iso/build/salsaos.iso iso/

# Run the OS in QEMU
run: iso
	qemu-system-x86_64 -cdrom iso/build/salsaos.iso

# Cleanup
clean:
	rm -rf entry.o kernel.o iso/build/kernel.elf iso/build/salsaos.iso
