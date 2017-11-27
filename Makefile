CC=gcc
LD=gcc
CFLAGS=-c -std=gnu11 -Wall -Wextra -g
LFLAGS=-lm
SRC=main.c matrix.c matrix_gauss.c matrix_uprel.c
OBJ=main.o matrix.o matrix_gauss.o matrix_uprel.o
TARGET=cm1

all: $(TARGET)

$(TARGET): $(OBJ)
	$(LD) $^ $(LFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

matrix.c : matrix.h

matrix_gauss.c: matrix_gauss.h

matrix_uprel.c: matrix_uprel.h

cleanup:
	rm -f $(OBJ) $(TARGET)

# Auxillary
tmaker: tmaker.c matrix.c
	gcc -g -std=gnu11 -Wall -Wextra $^ -lm -o $@
