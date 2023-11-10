rm a.out
clang ../lib/*.c *.c ../../deps/breathe/*.c ../../deps/tiny-libc/*.c -I ../../deps/ -I ../include
./a.out

