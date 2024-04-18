#include "matrix.h"

inline void transpose(double *B, size_t cols) {
    #pragma omp parallel for
    for (size_t i = 0; i < cols; i++) {
        for (size_t j = i+1; j < cols; j++) {
            double temp = B[i*cols+j];
            B[i*cols+j] = B[j*cols+i];
            B[j*cols+i] = temp;
        }
    }
}

double Matrix::calculate() {
    
    transpose(B, cols);
    
    double start = getTime();
    
    // Multiply
    for (size_t arg0 = 0; arg0<coo->nnz; arg0++) {
        size_t i = coo_rows[arg0];
        size_t j = coo_cols[arg0];
        double val = coo_vals[arg0];
        
        for (size_t k = 0; k<k_bound; k++) {
            C[i*cols+k] += val * B[k*cols+j];
        }
    }
    
    double end = getTime();
    
    transpose(B, cols);
    return (double)(end-start);
}

