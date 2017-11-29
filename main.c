#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"
#include "matrix_uprel.h"
#include "matrix_gauss.h"

#define EPS 1e-10
#define PRECISION_VAL 1e-8

void
print_solution(double *solution, int m, FILE *f_out);

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Specify command-line arguments (-h for help)\n");
        exit(1);
    }
    if (argv[1][0] == '-' && argv[1][1] == 'h') {
        // Print help
        printf("Command-line arguments:\n"
                "g for Gaussian method\n"
                "gm for Gaussian method with main element search\n"
                "u for Upper Relaxation method (next arg to specify w)\n");
        exit(0);
    }

    int m, n;
    matlib_init();
    scanf("%d", &m);
    n = m + 1; // extended matrix
    Matrix matrix;
    matrix_init(&matrix, m, n);
    
    matrix_read_file(&matrix, stdin);

    double *solution = calloc(m, sizeof(*solution));

    if (argv[1][0] == 'g') {
        // Gaussian method
        int main_search = 0;
        if (argv[1][1] == 'm') {
            main_search = 1;
        }
        if (!main_search) {
            printf("### Gaussian method ###\n");
        } else {
            printf("### Gaussian method with main element search ###\n");
        }
        Matrix matrix_inv;
        matrix_init(&matrix_inv, m, m);
        int *columns = NULL;
        int add_args = 0;
        if (main_search) {
            add_args = GF_MAIN_SEARCH | GF_COL_REC | GF_CALC_INVERSE;
            columns = calloc(m, sizeof(*columns));
        }
        int ret = matrix_gauss(&matrix,
                (GF_CALC_DET | GF_CALC_INVERSE) ^ add_args,
                &matrix_inv,
                columns);
        if (!ret) {
            printf("Could not execute Gaussian method\n");
            exit(0);
        }
        if (fabs(matrix.det) < EPS) {
            printf("The matrix is singular\n");
            exit(0);
        }
        if (!main_search) {
            // Now extract the solution from the matrix
            // Because the matrix is now unit, all we need to do
            // is to extract its last column
            for (int i = 0; i < m; ++i) {
                solution[i] = matrix.arr[i * n + (n - 1)]; // Last element of the matrix
            }
        } else {
            // In case we have searched for main elements, that is
            // have permuted columns, some backwards permutation is required
            for (int i = 0; i < m; ++i) {
                solution[columns[i]] = matrix.arr[i * n + (n - 1)];
            }
        }
        
        print_solution(solution, m, stdout);
        printf("Determinant: %12.5g\n", matrix.det);
        if (!main_search) {
            printf("Inverse matrix:\n");
            matrix_print_file(&matrix_inv, stdout);
        }
        matrix_destroy(&matrix_inv);
        if (main_search) {
            free(columns);
        }
    } else if (argv[1][0] == 'u') {
        // Upper Relaxation method
        printf("### Upper Relaxation method ###\n");
        double w = 1.0; // Zeidel by default
        if (argv[2]) {
            w = strtod(argv[2], NULL);
            if (w == 0) {
                fprintf(stderr, "Wrong w value\n");
                exit(1);
            }
        }
        int itr;
        int ret = matrix_uprel(&matrix, w, PRECISION_VAL, solution, &itr);
        if (!ret) {
            printf("Could not execute Upper Relaxation Method\n");
            exit(0);
        }
        print_solution(solution, m, stdout);
        printf("Number of iterations: %d\n", itr);
    } else {
        fprintf(stderr, "Wrong command-line arguments (-h for help)\n");
        exit(1);
    }

    free(solution);
    matrix_destroy(&matrix);
    matlib_exit();
    return 0;
}

void
print_solution(double *solution, int m, FILE *f_out)
{
    fprintf(f_out, "Solution:\n");
    for (int i = 0; i < m; ++i) {
        fprintf(f_out, "x%d = %12.5g\n", i + 1, solution[i]);
    }
}
