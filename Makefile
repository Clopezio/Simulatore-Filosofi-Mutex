CC = cc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2
LDLIBS = -pthread

.PHONY: all run clean

all: filosofi

filosofi: filosofi.c
	$(CC) $(CFLAGS) filosofi.c -o filosofi $(LDLIBS)

run: filosofi
	./filosofi

clean:
	$(RM) filosofi
