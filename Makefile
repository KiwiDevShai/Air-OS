C_SRC := $(shell find src -type f -name '*.c')
ASM_SRC := $(shell find src -type f -name '*.asm')

C_OBJ := $(C_SRC:src/%.c=build/%.o)
ASM_OBJ := $(ASM_SRC:src/%.asm=build/%.o)

OBJ := $(ASM_OBJ) $(C_OBJ)

all: airos.iso

airos.iso: build/airos.bin
	cp build/airos.bin iso_setup/boot/airos.bin
	grub-mkrescue -o airos.iso iso_setup

build/airos.bin: $(OBJ)
	ld -m elf_i386 -T linker.ld -o $@ $(OBJ)

build/%.o: src/%.asm
	mkdir -p $(dir $@)
	nasm -f elf32 $< -o $@

run: airos.iso
	qemu-system-i386 -cdrom airos.iso

clean:
	rm -rf build/* airos.iso iso_setup/boot/airos.bin