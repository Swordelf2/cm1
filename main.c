#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "matrix_uprel.h"
#include "matrix_gauss.h"

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Specify command-line arguments\n");
    }
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

    if (*argv[1] == 'u') { // upper relaxation
        int res = matrix_uprel(&matrix, 1, 0.000000001, solution, &itr);

        printf("### Upper relaxation method ###\n"
                "Return value: %d\n", res);
        printf("Solution:\n");
        for (int i = 0; i < m; ++i) {
            printf("x%d = %10.5g\n", i + 1, solution[i]);
        }
        printf("Number of iterations = %d\n", itr);
    } else {
        // Gauss method with main search
        int *columns = calloc(m, sizeof(*columns));
        int res = matrix_gauss(&matrix, GF_MAIN_SEARCH | GF_COL_REC | GF_CALC_DET, NULL, columns);
        for (int i = 0; i < m; ++i) {
            solution[columns[i]] = matrix.arr[i * (m + 1) + m];
        }
        printf("### Gaussian method ###\n"
                "Return value: %d\n", res);
        printf("Solution:\n");
        for (int i = 0; i < m; ++i) {
            printf("x%d = %10.5g\n", i + 1, solution[i]);
        }
        printf("Determinant:%10.5g\n", matrix.det);
        free(columns);
    }
    free(solution);
    matrix_destroy(&matrix);
    matlib_exit();
    return 0;
}
