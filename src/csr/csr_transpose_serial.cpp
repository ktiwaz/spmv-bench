#include "matrix.h"

inline void transpose(double *B, size_t cols) {
    for (size_t i = 0; i < cols; i++) {
        for (size_t j = i+1; j < cols; j++) {
            double temp = B[i*cols+j];
            B[i*cols+j] = B[j*cols+i];
            B[j*cols+i] = temp;
        }
    }
}

//
// The calculation algorithm for the current format
//
double Matrix::calculate() {
    double start = getTime();
    
    transpose(B, cols);
    
    for (size_t i = 0; i<rows; i++) {
        for (uint64_t p = rowptr[i]; p<rowptr[i+1]; p++) {
            uint64_t j = rowidx[p];
            for (uint64_t k = 0; k<k_bound; k++) {
                C[i*cols+k] += values[p] * B[k*cols+j];
            }
        }
    }
    
    double end = getTime();
    transpose(B, cols);
    return (double)(end-start);
}

