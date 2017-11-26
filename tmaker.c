#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "matrix.h"

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Specify command-line arguments (-help for help)\n");
        return 1;
    }

    if (strcmp(argv[1], "-help") == 0) {
        printf("### First argument:\n"
                "0 for square matrix\n"
                "1 for extended\n"
                "### Second argument (not required):\n"
                "Number of lines/columns\n"
                "### Third argument  (not required):\n"
                "Seed for the rand thing\n");
    }
    
    unsigned seed;
    if (argc >= 1 + 3) {
        seed = strtol(argv[3], NULL, 10);
    } else {
        seed = time(NULL);
    }
    srand(seed);

    int m, n;
    if (argc >= 1 + 2) {
        m = strtol(argv[2], NULL, 10);
    } else {
        m = rand() % 5 + 3;
    }

    double *x = malloc(m * sizeof(*x));
    for (int i = 0; i < m; ++i) {
        x[i] = (int) (rand() / (RAND_MAX + 1.0) * 10.0) - 5.0;
    }

    if (*argv[1] == '0') {
        n = m;
    } else {
        n = m + 1;
    }

    matlib_init();
    Matrix matrix;
    matrix_init(&matrix, m, n);
    for (int i = 0; i < m; ++i) {
        double b = 0.0;
        for(int j = 0; j < m; ++j) {
            double val = (int) (rand() / (RAND_MAX + 1.0) * 10.0) - 5.0;
            b += val * x[j];
            matrix.arr[i * n + j] = val;
        }
        if (n > m) {
            matrix.arr[i * n + m] = b;
        }
    }

    FILE *f = fopen("t/gen", "w+");
    fprintf(f, "%d\n", m);
    matrix_print_file(&matrix, f);
    if (*argv[1] == '1') {
        fprintf(f, "\nSolution:\n");
        for (int i = 0; i < m; ++i) {
            fprintf(f, "x%d = %.10g\n", i + 1, x[i]);
        }
    }

    fclose(f);

    matrix_destroy(&matrix);
    matlib_exit();
    free(x);
    return 0;
}
