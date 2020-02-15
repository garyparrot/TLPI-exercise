gcc -c -g -fPIC vis1.c vis2.c vcomm.c
gcc -g -shared -o libvis.so vcomm.o vis1.o vis2.o -Wl,--version-script,vis.map

readelf --syms --use-dynamic libvis.so | grep 'func\|v_comm'

rm *.o *.so
