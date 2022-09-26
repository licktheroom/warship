# Copyright (C) 2022  licktheroom #
# new file!
# basically the same!

ifeq (, $(shell which clang))
CC = gcc
CFLAGS = -O2 -march=native -pipe -fomit-frame-pointer -Wall -Wextra -Wshadow -Wdouble-promotion -fno-common
else
CC = clang
CFLAGS = -O2 -march=native -pipe -fomit-frame-pointer -Wall -Wextra -Wshadow -Wdouble-promotion -fno-common -ferror-limit=10
endif

objects = main.o gl.o vector.o physics.o settings.o

all: make_clean

main.o: main.c
gl.o: gl.c
vector.o: vector.c
physics.o: physics.c
settings.o: settings.c

release: ${objects}
	${CC} ${CFLAGS} -lglfw -ldl -lm -o warship ${objects}

make_clean: release
	rm -f *.o

clean:
	rm -f *.o
