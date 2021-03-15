.PHONY: run clean

CC=x86_64-elf-gcc
CFLAGS=-ffreestanding -Wall -Wextra -Wno-int-conversion \
		-gdwarf                         \
		-isystem src                    \
		-fno-pic                        \
		-mcmodel=kernel                 \
		-mno-sse                        \
		-mno-sse2                       \
		-mno-mmx                        \
		-mno-80387                      \
		-mno-red-zone                   \
		-fno-stack-protector            \
		-fno-omit-frame-pointer         \

SOURCES = $(shell find src/ -type f -name '*.c')
HEADERS = $(shell find src/ -type f -name '*.h')

ASMFILE = $(shell find src/ -type f -name '*.asm')
OBJECTS = ${SOURCES:.c=.o} ${ASMFILE:.asm=.o}

celesteos.iso: kernel.elf
	@mkdir -p build/iso/boot/grub
	@cp kernel.elf build/iso/boot/kernel.bin
	@cp grub.cfg build/iso/boot/grub
	@grub-mkrescue -o build/celesteos.iso build/iso
	#rm -r build/iso

kernel.elf: ${OBJECTS}
	x86_64-elf-ld -n -T linker.ld -z max-page-size=0x1000 -nostdlib -o $@ ${OBJECTS}

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -nostdlib -o $@

%.o: %.asm
	nasm -i src/kernel -felf64 -F dwarf -g $< -o $@

run: celesteos.iso
	@qemu-system-x86_64 -smp cpus=4 -cdrom build/celesteos.iso -m 512M -no-reboot -debugcon stdio -d int -D qemu.log -no-shutdown -vga vmware

clean:
	-rm kernel.elf
	-rm build/celesteos.iso
	-rm ${OBJECTS}
	-rm qemu.log
	-rm mem
	-rm dump
