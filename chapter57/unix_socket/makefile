.DEFAULT_GOAL := compile 
directory := $(realpath ../../lib)

compile:
	gcc -I $(directory) $(directory)/*.c ./usocket_client.c -o client.out
	gcc -I $(directory) $(directory)/*.c ./usocket_server.c -o server.out

clean:
	rm *.out
	rm *.o
