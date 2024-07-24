clang -c -target mipsel-none-elf -nostdlib  test1.s
ld.lld test1.o  -o test1.elf

clang -c -target mipsel-none-elf -nostdlib  test1.s
ld.lld test1.o --oformat binary  -o test1.bin

clang -c -target mipsel-none-elf -nostdlib crt0.s

clang -c -target mipsel-none-elf -nostdlib test2.c
ld.lld crt0.o test2.o  -o test2.elf

clang -c -target mipsel-none-elf -nostdlib test2.c
ld.lld crt0.o test2.o --oformat binary  -o test2.bin
