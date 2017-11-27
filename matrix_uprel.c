#include "matrix.h"
#include "matrix_uprel.h"

int
matrix_uprel(Matrix *matrix, double w, double eps)
{
    // Some initializaion
    double *mat
