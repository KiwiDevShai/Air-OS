# Air-OS

Air-OS is a hobby operating system.

## How to Use

### Dependencies
- cc
- ld
- nasm
- grub-mkrescue
- qemu-system-x86_64
- make

### Run
```bash
make run
```

### Build
```bash
make
```

### Clean
```bash
make clean
```

## Credits
- `printf.c` / `printf.h` are based on [eyalroz/printf](https://github.com/eyalroz/printf), a tiny standalone printf implementation licensed under the MIT License.
- `multiboot2.h` is GNU GRUB's Multiboot2 specification header.