#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "matrix_uprel.h"

int
main(void)
{
    int n;
    int m;
    matlib_init();
    scanf("%d", &n);
    m = n;
    Matrix matrix;
    matrix_init(&matrix, n, n + 1);
    matrix_read_file(&matrix, stdin);

    double *solution = calloc(m, sizeof(*solution));
    int itr = -1;

    matrix_uprel(&matrix, 1, 0.001, solution, &itr);

    printf("Solution:\n");
    for (int i = 0; i < m; ++i) {
        printf("x%d = %10.5g\n", i + 1, solution[i]);
    }
    printf("\nNumber of iterations = %d\n", itr);

    matrix_destroy(&matrix);
    matlib_exit();
    return 0;
}
