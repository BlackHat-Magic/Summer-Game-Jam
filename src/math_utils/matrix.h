#ifndef PIRATE_MATRIX
#define PIRATE_MATRIX

typedef struct {
    int rows;
    int cols;
    double* data;
} Matrix;

Matrix* matrix_create (int rows, int cols);
void matrix_free (Matrix* m);
Matrix* matrix_multiply (const Matrix* a, const Matrix* b);
Matrix* matrix_add (const Matrix* a, const Matrix* b);

#endif