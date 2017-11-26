#include <stdio.h>

typedef struct Matrix
{
    double *arr; // memory where the matrix is stored
    int m; // number of lines
    int n; // number of columns
    double det;
} Matrix;

extern double *buf;
// A buffer for certain operations on matrices

extern int buf_size;

void
matlib_init(void);
// Initilizes some global variables, has to be called before any further mat calls

void
matrix_init(Matrix *matrix, int m, int n);
// Initialize matrix

void
matrix_unit(Matrix *matrix);
// Make the matrix a unit matrix

void
matrix_read_file(Matrix *matrix, FILE *file);
// Read the matrix from file, given m and n are already initialized

void
matrix_print_file(const Matrix *matrix, FILE *file);
// Prints the matrix into file

void
matrix_swap_lines(Matrix *matrix, int line1, int line2);

void
matrix_swap_columns(Matrix *matrix, int column1, int column2);

void
matrix_subtract_line(Matrix *matrix, int line1, int line2, double k);
// susbstracts line2 * k from line1

void
matrix_destroy(Matrix *matrix);
// Free matrix memory

void
matlib_exit(void);
