a.exe: main.o stack.o
	g++ main.o stack.o
main.o: main.c
	g++ -c main.c
stack.o: stack.c
	g++ -c stack.c