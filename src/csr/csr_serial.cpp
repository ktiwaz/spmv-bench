#include "matrix.h"

double _calculate(size_t rows, uint64_t *rowptr, uint64_t *rowidx, double *values, double *C, double *B, uint64_t cols) {
    double start = SpM::getTime2();

    // Perform SpMV by iterating over rows and non-zero elements
    for (size_t i = 0; i < rows; i++) {
        for (uint64_t p = rowptr[i]; p < rowptr[i + 1]; p++) {
            uint64_t j = rowidx[p];
            double v = values[p];
            // Perform the SpMV operation: C[i] += values[p] * B[j]
            C[i] += v * B[j];
        }
    }

    double end = SpM::getTime2();
    return (double)(end - start);
}

//
// The calculation algorithm for SpMV
//
double Matrix::calculate() {
    // Call the SpMV calculation for the sparse matrix
    return _calculate(rows, rowptr, rowidx, values, C, B, cols);
}

