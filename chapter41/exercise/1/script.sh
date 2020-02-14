gcc         code.c -o nostatic
gcc -static code.c -o static

du -h static
ldd static
du -h nostatic
ldd nostatic

rm static nostatic
