#include <stdlib.h>
#include "matrix.h"

Matrix* matrix_create (int rows, int cols) {
    Matrix* m = (Matrix*) malloc (sizeof (Matrix));
    if (m == NULL) return NULL;

    m->rows = rows;
    m->cols = cols;

    m->data = (double*) calloc (rows * cols, sizeof (double));
    if (m->data == NULL) return NULL;

    return m;
}

void matrix_free (Matrix* m) {
    if (m == NULL) return;
    free (m->data);
    free (m);
}

Matrix* matrix_multiply (const Matrix* a, const Matrix* b) {
    // dimensions must match
    if (a->cols != b->rows) return NULL;

    Matrix* result = matrix_create (a->rows, b->cols);
    if (result == NULL) return NULL;

    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            // we can skip initializing matrix to zero because calloc already does that
            for (int k = 0; k < a->cols; k++) {
                result->data[i * result->cols + j] += 
                    a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
        }
    }
    return result;
}

Matrix* matrix_add (const Matrix* a, const Matrix* b) {
    // dimensions must match
    if (a->cols != b->cols || a->rows != b->rows) return NULL;

    Matrix* result = matrix_create (a->rows, b->rows);
    if (result == NULL) return NULL;

    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a-> rows; j++) {
            result->data[i * a->rows + j] = a->data[i * a->rows + j] + b->data[i * a->rows + j];
        }
    }
    return result;
}