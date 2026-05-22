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
	ld -m elf_x86_64 -T linker.ld -o $@ $(OBJ)

build/%.o: src/%.c
	mkdir -p $(dir $@)
	cc -Iinclude -Iinclude/vendor -m64 -ffreestanding -fno-stack-protector -mno-red-zone -c $< -o $@

build/%.o: src/%.asm
	mkdir -p $(dir $@)
	nasm -f elf64 $< -o $@

run: airos.iso
	qemu-system-x86_64 -cdrom airos.iso

clean:
	rm -rf build/* airos.iso iso_setup/boot/airos.bin