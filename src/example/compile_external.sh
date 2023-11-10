rm a.out
deps=../../external
deps_lib=$deps/lib
deps_include=$deps/include
clang ../lib/*.c *.c $deps_lib/posix/*.c $deps_lib/tiny-libc/*.c -I $deps_include -I ../include
./a.out

