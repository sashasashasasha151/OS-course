all: main libdinf.so libdins.so

static_f.o: static_f.c
	gcc -c static_f.c -o static_f.o

static_s.o: static_s.c
	gcc -c static_s.c -o static_s.o

libstatic.a: static_s.o static_f.o
	ar rc libstatic.a static_s.o static_f.o

dinamic_f.o: dinamic_f.c
	gcc -c -Wall dinamic_f.c -o dinamic_f.o

libdinf.so: dinamic_f.o
	gcc dinamic_f.o -shared -fpic -o libdinf.so

dinamic_s.o: dinamic_s.c
	gcc -c -Wall -Werror dinamic_s.c -o dinamic_s.o

libdins.so: dinamic_s.o
	gcc dinamic_s.o -shared -fpic -o libdins.so

main.o: main.c
	gcc -c main.c -o main.o

main: main.o libdinf.so libstatic.a
	gcc -L. -Wl,-rpath,. main.o libstatic.a -o main -ldinf -ldl