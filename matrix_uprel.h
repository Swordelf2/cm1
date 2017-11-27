#ifndef MATRIX_UPREL_H
#define MATRIX_UPREL_H

#include "matrix.h"

// Upper relaxation method

int
matrix_uprel(Matrix *matrix, double w, double eps, double *solution, int *itr);
// The solution is stored in solution
// The number of iterations is stored in it
// If this function fails (the solutions don't converge) it returns 0

#endif
