all: main.o lib.o dict.o
	ld main.o lib.o dict.o -o lab2

main.o: colon.inc main.asm
	nasm -felf64 -o main.o main.asm

lib.o: lib.inc
	nasm -felf64 -o lib.o lib.inc

dict.o: dict.asm
	nasm -felf64 -o dict.o dict.asm

clean:
	rm -rf *.o lab2