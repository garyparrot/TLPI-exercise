# Compile library
gcc -g -c -fPIC -Wall sv_lib_v2.c
gcc -g -shared -o libsv.so sv_lib_v2.o -Wl,--version-script,sv_v2.map

# Compile code
gcc -g -o p2 sv_prog.c libsv.so

# Result
env LD_LIBRARY_PATH=. ./p2
env LD_LIBRARY_PATH=. ./p1

# Check symbol version
objdump -t p1 | grep xyz
objdump -t p2 | grep xyz

rm *.o 
