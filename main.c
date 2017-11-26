#include <stdio.h>

#include "matrix.h"

int
main(void)
{
    int n, m, line1, line2;
    matlib_init();
    scanf("%d %d", &n, &m);
    Matrix matrix;
    matrix_init(&matrix, n, m);
    matrix_read_file(&matrix, stdin);
    scanf("%d %d", &line1, &line2);
    matrix_swap_columns(&matrix, line1, line2);
    matrix_print_file(&matrix, stdout);
    matrix_destroy(&matrix);
    matlib_exit();
    return 0;
}
