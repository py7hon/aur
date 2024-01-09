SOURCE  = aur.c
OUT     = aur.o
CC      = gcc
LIBS    = -lgit2

all: build copy

copy:
	cp aur.o /usr/bin/aur

build: aur.c
	$(CC) $(SOURCE) -o aur.o $(LIBS)

clean:
	rm -f $(OUT)
	rm -f /usr/bin/aur
