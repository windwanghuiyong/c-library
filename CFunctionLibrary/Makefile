static:
	gcc -c hello.c
	ar rcs libhello.a hello.o
	gcc main.c -lhello -L ./ -static -o hello

cleanstatic:
	rm -f *.o *.a hello

shared:
	gcc -c hello.c
	gcc -shared -fPIC -o libhello.so hello.o
	gcc -o hello main.c -L ./ -lhello
	cp libhello.so /usr/lib
	export LD_LIBRARY_PATH=./

cleanshared:
	rm -f *.o *.a hello
