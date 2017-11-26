CC=gcc
LD=gcc
CFLAGS=-c -std=gnu11 -Wall -Wextra -g
LFLAGS=
SRC=main.c matrix.c
OBJ=main.o matrix.o
TARGET=cm1

all: $(TARGET)

$(TARGET): $(OBJ)
	$(LD) $(LFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

matrix.c : matrix.h

cleanup:
	rm -f $(OBJ) $(TARGET)
