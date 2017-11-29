#include "matrix.h"
#include "matrix_gauss.h"
#include <math.h>
#include <string.h>

void
matrix_swap_lines(Matrix *matrix, int line1, int line2)
{
    double *l1_start = matrix->arr + line1 * matrix->n;
    double *l2_start = matrix->arr + line2 * matrix->n;
    memcpy(buf, l1_start, matrix->n * sizeof(*buf));
    memcpy(l1_start, l2_start, matrix->n * sizeof(*l1_start));
    memcpy(l2_start, buf, matrix->n * sizeof(*l2_start));
    matrix->det = -matrix->det;
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
    matrix->det = -matrix->det;
}

void
matrix_substract_line(Matrix *matrix, int line1, int line2, double k)
{
    int n = matrix->n;
    for (int j = 0; j < n; ++j) {
        matrix->arr[line1 * n + j] -= k * matrix->arr[line2 * n + j];
    }
}

void
matrix_multiply_line(Matrix *matrix, int line, double k, int first_nz, int flag)
{
    int n = matrix->n;
    for (int j = first_nz; j < n; ++j) {
        matrix->arr[line * n + j] *= k;
    }
    if (flag) {
        matrix->det /= k;
    }
}

int
matrix_gauss(Matrix *matrix, int flags, Matrix *inv_matrix, int *columns)
{
    if ((flags & GF_CALC_INVERSE) && (flags & GF_MAIN_SEARCH)) {
        return 0;
    }
    // Perfrom some initialization
    int m = matrix->m;
    int n = matrix->n;
    double *mat = matrix->arr;
    if (flags & GF_CALC_INVERSE) {
        matrix_unit(inv_matrix);
    }
    if (flags & GF_CALC_DET) {
        matrix->det = 1.0;
    }
    if (flags & GF_COL_REC) {
        for (int i = 0; i < m; ++i) {
            columns[i] = i;
        }
    }
    
    // Actuall Gaussian method
    for (int it = 0; it < m; ++it) {
        if (flags & GF_MAIN_SEARCH) {
            // Search for main element (with the largest absolute value)
            double max_val = mat[it * n + it];
            int max_ind = it;
            for (int j = it + 1; j < m; ++j) {
                double abs_val = fabs(mat[it * n + j]);
                if (abs_val > max_val) {
                    max_val = abs_val;
                    max_ind = j;
                }
            }
            if (it != max_ind) {
                // Swap the column with that element with the current column
                matrix_swap_columns(matrix, it, max_ind);
                if (flags & GF_CALC_INVERSE) {
                    // Same swap for inv_marix
                    matrix_swap_columns(inv_matrix, it, max_ind);
                }
                if (flags & GF_COL_REC) {
                    int tmp = columns[it];
                    columns[it] = columns[max_ind];
                    columns[max_ind] = tmp;
                }
            } else if (mat[it * n + it] == 0.0) {
                matrix->det = 0.0;
                return 0;
            }
        } else {
            // Search for the first non-zero element in the current column
            int nz_ind;
            for (nz_ind = it;
                    nz_ind < m && mat[nz_ind * n + it] == 0.0;
                    ++nz_ind);
            if (nz_ind == m) {
                matrix->det = 0.0;
                return 0;
            }
            // Swap that line with the current one
            matrix_swap_lines(matrix, it, nz_ind);
            if (flags & GF_CALC_INVERSE) {
                // Same swap for inv_matrix
                matrix_swap_lines(inv_matrix, it, nz_ind);
            }
        }

        // Divide this line by the main element
        double mul_val = 1.0 / mat[it * n + it];
        matrix_multiply_line(matrix,
                it,
                mul_val,
                it,
                flags & GF_CALC_DET);
        if (flags & GF_CALC_INVERSE) {
            // Same for the inverse matrix
            matrix_multiply_line(inv_matrix,
                    it,
                    mul_val,
                    0,
                    0);
        }
        // Now substract this line from all those beneath it
        for (int i = it + 1; i < m; ++i) {
            double mul_val = mat[i * n + it];
            matrix_substract_line(matrix, i, it, mul_val);
            if (flags & GF_CALC_INVERSE) {
                // Same for the inverse matrix
                matrix_substract_line(inv_matrix, i, it, mul_val);
            }
        }
    }
    
    // if we only need to calculated det (with maybe MAIN_SEARCH flag
    if ((flags | GF_MAIN_SEARCH) == (GF_CALC_DET | GF_MAIN_SEARCH)) {
        // Then exit, job is done
        return 1;
    }

    // Reverse
    for (int it = m - 1; it > 0; --it) {
        double ext_val = mat[it * n + m];
        // Substract this line from all above it
        for (int i = it - 1; i >= 0; --i) {
            // if (n > m) means that if the matrix is extended we peform the substraction
            // of the extended part
            if (n > m) {
                mat[i * n + m] -= mat[i * n + it] * ext_val;
            }
            if (flags & GF_CALC_INVERSE) {
                matrix_substract_line(inv_matrix, i, it, mat[i * n + it]);
            }
            mat[i * n + it] = 0.0;
        }
    }
    return 1;
}
