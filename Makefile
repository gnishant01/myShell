CC = gcc
CFLAG = -Wall
SOURCE = myShell.c
OUTPUT = myShell

build: $(FILES)
	$(CC) $(CFLAG) -o $(OUTPUT) $(SOURCE)

clean:
	rm -f *.o core

rebuild: clean build
