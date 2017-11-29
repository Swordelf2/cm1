#include "matrix.h"
#include <stdlib.h>

double *buf = NULL;
int buf_size = 0;

void
matlib_init(void)
{
    buf_size = BUF_INIT_SIZE;
    buf = malloc(buf_size * sizeof(*buf));
}

void
matrix_init(Matrix *matrix, int m, int n)
{
    matrix->m = m;
    matrix->n = n;
    matrix->arr = malloc(m * n * sizeof(*matrix->arr));
    matrix->det = 0.0;
    // Extend the buffer if needed
    int max_num = (m > n) ? m : n;
    if (max_num > buf_size) {
        buf = realloc(buf, max_num * sizeof(*buf));
    }
}

void
matrix_read_file(Matrix *matrix, FILE *file)
{
    int m = matrix->m;
    int n = matrix->n;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            fscanf(file, "%lf", &(matrix->arr[i * n + j]));
        }
    }
}

void
matrix_print_file(const Matrix *matrix, FILE *file)
{
    int m = matrix->m;
    int n = matrix->n;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            fprintf(file, "%12.5g ", matrix->arr[i * n + j]);
        }
        putc('\n', file);
    }
}

void
matrix_unit(Matrix *matrix)
{
    int m = matrix->m;
    int n = matrix->n;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix->arr[i * n + j] = (i == j) ? 1.0 : 0.0;
        }
    }
}

void
matrix_destroy(Matrix *matrix)
{
    free(matrix->arr);
}

void
matlib_exit(void)
{
    free(buf);
}
