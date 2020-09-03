SOURCE  = aur.c
OUT    	= aur.o
CC	= gcc

all: build copy

copy:
	cp aur.o /usr/bin/aur


build: aur.c
	$(CC) $(SOURCE) -o aur.o


clean:
	rm -f $(OUT)
	rm -f /usr/bin/aur
