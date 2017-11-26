#include "matrix.h"
#include <stdlib.h>
#include <string.h>

double *buf = NULL;
int buf_size = 0;

enum
{
    BUF_INIT_SIZE = 16
};

enum GaussFlags
{
    GF_MAIN_SEARCH = 0x1u, // whether to search for main element in a line
    GF_CALC_DET = 0x2u, // whether to calculate determinant
    GF_CALC_INVERSE = 0x4u // whether to calculate inverse matrix
};

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
            fprintf(file, "%10.10g ", matrix->arr[i * n + j]);
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
matrix_swap_lines(Matrix *matrix, int line1, int line2)
{
    double *l1_start = matrix->arr + line1 * matrix->n;
    double *l2_start = matrix->arr + line2 * matrix->n;
    memcpy(buf, l1_start, matrix->n * sizeof(*buf));
    memcpy(l1_start, l2_start, matrix->n * sizeof(*l1_start));
    memcpy(l2_start, buf, matrix->n * sizeof(*l2_start));
}

void
matrix_swap_columns(Matrix *matrix, int column1, int column2)
{
    int m = matrix->m;
    int n = matrix->n;
    for (int i = 0; i < m; ++i) {
        // swap arr[i][column1] and arr[i][column2]
        double temp = matrix->arr[i * n + column1];
        matrix->arr[i * n + column1] = matrix->arr[i * n + column2];
        matrix->arr[i * n + column2] = temp;
    }
}

void
matrix_subtract_line(Matrix *matrix, int line1, int line2, double k)
{
    int n = matrix->n;
    for (int j = 0; j < n; ++j) {
        matrix->arr[line1 * n + j] -= k * matrix->arr[line2 * n + j];
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
