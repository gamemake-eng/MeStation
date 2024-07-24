clang -c -target mips-none-elf -nostdlib test1.s
ld.lld test1.o -o test1.elf
