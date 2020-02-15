# Compile library
gcc -g -c -fPIC -Wall sv_lib_v1.c
gcc -g -shared -o libsv.so sv_lib_v1.o -Wl,--version-script,sv_v1.map

# Compile code
gcc -g -o p1 sv_prog.c libsv.so

# Result
env LD_LIBRARY_PATH=. ./p1

rm *.o
