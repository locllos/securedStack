a.exe: main.o stack.o
	clang main.o stack.o
main.o: main.c
	clang -c main.c
stack.o: stack.c
	clang -c stack.c