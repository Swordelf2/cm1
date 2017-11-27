#include "matrix.h"
#include "matrix_uprel.h"

#include <stdlib.h>
#include <string.h>

enum UprelConstants
{
    MAX_ITERATIONS = 10000
};

int
matrix_uprel(Matrix *matrix, double w, double eps, double *solution, int *itr)
{
    // The formula is as follows
    // Dx = -wLx + (1-w)Dx_p - wRx_p + wb
    //       (1)     (2)         (3)     (4)
    // where x_p is x_prev - x from the previous iteration
    // x is x_cur - x being constructed on current iteration
    // D R L are respective parts of the matrix

    // Some initializaion
    // wL and wR will both be stored in matrix wmat
    // Also (1-w)D will also be stored in that same matrix
    // Dmat is (1-w)D
    // diag is the diagonal elements of the given matrix
    // wb is wb


    int m = matrix->m;
    int n = matrix->n;
    double *mat = matrix->arr;
    double *x_cur = calloc(m, sizeof(*x_cur));
    double *x_prev = calloc(m, sizeof(*x_prev));
    // Init wmat
    double *wmat = calloc(m * m, sizeof(*wmat));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            if (i != j) {
                wmat[i * m + j] = mat[i * n + j] * w;
            } else {
                wmat[i * m + j] = mat[i * n + j] * (1 - w);
            }
        }
    }
    // Init wb
    double *wb = calloc(m, sizeof(*wb));
    for (int i = 0; i < m; ++i) {
        wb[i] = mat[i * n + m] * w;
    }
    // Init diag
    double *diag = calloc(m, sizeof(*diag));
    for (int i = 0; i < m; ++i) {
        diag[i] = mat[i * n + i];
    }

    int success_flag = 0;
    int it;
    for (it = 0; it < MAX_ITERATIONS; ++it) {
        // See comments for explanation on (1), ..., (4)
        for (int i = 0; i < m; ++i) {
            double comp = 0.0; // i-th component
            // (1)
            for (int j = 0; j < i; ++j) {
                comp -= wmat[i * m + j] * x_cur[j];
            }
            // (2)
            comp += wmat[i * m + i] * x_prev[i];
            // (3)
            for (int j = i + 1; j < m; ++j) {
                comp -= wmat[i * m + j] * x_prev[j];
            }
            comp += wb[i];

            x_cur[i] = comp / diag[i];
        }

        // Now we calculate the norm of the difference of x_cur and x_prev
        double diff_norm = 0.0;
        for (int i = 0; i < m; ++i) {
            double diff = x_cur[i] - x_prev[i];
            diff_norm += diff * diff;
        }

        if (diff_norm < eps) {
            success_flag = 1;
            break;
        }
    }
        
    if (success_flag) {
        if (solution) {
            memcpy(solution, x_prev, m * sizeof(*solution));
        }
        if (itr) {
            *itr = it;
        }
    }
    free(x_cur);
    free(x_prev);
    free(wmat);

    return success_flag;
}
