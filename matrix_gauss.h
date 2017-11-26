#ifndef MATRIX_GAUSS_H
#define MATRIX_GAUSS_H

#include "matrix.h"

enum GaussFlags
{
    GF_MAIN_SEARCH = 0x1, // whether to search for main element in a line
    GF_CALC_DET = 0x2, // whether to calculate determinant
    GF_CALC_INVERSE = 0x4, // whether to calculate inverse matrix
    GF_COL_REC = 0x8 // whether to record column swaps
};

void
matrix_swap_lines(Matrix *matrix, int line1, int line2);

void
matrix_swap_columns(Matrix *matrix, int column1, int column2);

void
matrix_substract_line(Matrix *matrix, int line1, int line2, double k);
// Susbstracts line2 * k from line1

void
matrix_multiply_line(Matrix *matrix, int line, double k, int first_nz, int flag);
// Multiplies line in matrix by k; first_nz is the index of
// the first non-zero element; flag is whether to modify matrix->det

void
matrix_gauss(Matrix *matrix, int flags, Matrix *inv_matrix, int *columns);
// Performs Gaussian method on matrix
// For notes of flags see "enum GaussFlags"
// inv_matrix is where the resulting inverse matrix is stored (if requested)
// columns is the array where the column swaps are stored (if requested)

#endif
