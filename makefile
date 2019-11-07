SRC=main.c track.c

all: 
	gcc -o track `pkg-config --libs --cflags glib-2.0 gtk+-2.0` -lm $(SRC)
