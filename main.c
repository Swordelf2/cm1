#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "matrix_gauss.h"

int
main(void)
{
    int n;
    matlib_init();
    scanf("%d", &n);
    Matrix matrix, inv_matrix;
    matrix_init(&matrix, n, n);
    matrix_init(&inv_matrix, n, n);
    matrix_read_file(&matrix, stdin);

    int *columns = malloc(n * sizeof(*columns));

    matrix_gauss(&matrix,
            GF_CALC_INVERSE,
            &inv_matrix,
            columns);
    printf("Inverse matrix\n");
    matrix_print_file(&inv_matrix, stdout);
    printf("Columns:\n");
    for (int i = 0; i < n; ++i) {
        printf("%d ", columns[i]);
    }
    printf("\nDeterminant: %.10g\n", matrix.det);
    free(columns);
    matrix_destroy(&matrix);
    matlib_exit();
    return 0;
}
