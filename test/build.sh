clang -c -target mipsel-none-elf -nostdlib  test1.s
ld.lld test1.o  -o test1.elf

clang -c -target mipsel-none-elf -nostdlib  test1.s
ld.lld test1.o --oformat binary  -o test1.bin
