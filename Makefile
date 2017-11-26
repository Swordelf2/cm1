CC=gcc
LD=gcc
CFLAGS=-c -std=gnu11 -Wall -Wextra -g
LFLAGS=-lm
SRC=main.c matrix.c matrix_gauss.c
OBJ=main.o matrix.o matrix_gauss.o
TARGET=cm1

all: $(TARGET)

$(TARGET): $(OBJ)
	$(LD) $^ $(LFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

matrix.c : matrix.h

matrix_gauss.c: matrix_gauss.h

cleanup:
	rm -f $(OBJ) $(TARGET)
