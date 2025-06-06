CROSS_COMPILE := x86_64-elf-
CC            := $(CROSS_COMPILE)gcc
AS            := nasm
LD            := ld

# freestanding + libc headers
CFLAGS        := -ffreestanding -mno-red-zone -m64 -nostdlib -Wall -Wextra \
                 -I src/kernel/libc -I src/kernel
LDFLAGS       := -n -T src/boot/linker.ld

# all object files
OBJS := header.o entry.o kernel.o stdlib.o stdio.o

all: iso/build/kernel.elf iso

#---------------------------------------------------
# Assemble boot-time stubs
#---------------------------------------------------
header.o: src/boot/header.asm
	$(AS) -f elf64 $< -o $@

entry.o:  src/boot/entry.asm
	$(AS) -f elf64 $< -o $@

#---------------------------------------------------
# Compile kernel + custom libc
#---------------------------------------------------
kernel.o: src/kernel/kernel.c \
          src/kernel/libc/stdlib.h \
          src/kernel/libc/stddef.h
	$(CC) $(CFLAGS) -c $< -o $@

stdlib.o: src/kernel/libc/stdlib.c \
          src/kernel/libc/stdlib.h \
          src/kernel/libc/stddef.h
	$(CC) $(CFLAGS) -c $< -o $@

stdio.o: src/kernel/libc/stdio.c \
		 src/kernel/libc/stdio.h \
		 src/kernel/libc/stddef.h
	$(CC) $(CFLAGS) -c $< -o $@

#---------------------------------------------------
# Link into a freestanding ELF
#---------------------------------------------------
iso/build/kernel.elf: $(OBJS)
	mkdir -p iso/build
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

#---------------------------------------------------
# Package with GRUB
#---------------------------------------------------
iso: iso/build/kernel.elf
	# ensure the GRUB folder exists (it’s already versioned in your repo)
	mkdir -p iso/boot/grub
	# symlink the freshly built kernel
	ln -sf ../build/kernel.elf iso/boot/kernel.elf
	# generate the bootable ISO
	grub2-mkrescue -o iso/build/salsaos.iso iso/ || true
	rm -f header.o entry.o kernel.o stdlib.o stdio.o
	
#---------------------------------------------------
# Run & Clean
#---------------------------------------------------
run: iso
	qemu-system-x86_64 -cdrom iso/build/salsaos.iso

clean:
	rm -f header.o entry.o kernel.o stdlib.o
	rm -rf iso/build/*
