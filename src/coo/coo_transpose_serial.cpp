#include "matrix.h"

double Matrix::calculate() {
    double start = getTime();
    
    // Transpose
    double* B_trans = new double[rows * cols];

    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        B_trans[j * rows + i] = B[i*cols+j];
      }
    }
    
    // Multiply
    for (size_t arg0 = 0; arg0<coo->nnz; arg0++) {
        size_t i = coo_rows[arg0];
        size_t j = coo_cols[arg0];
        double val = coo_vals[arg0];
        for (size_t k = 0; k<cols; k++) {
            C[i*cols+j] += val * B_trans[j*cols+k];
        }
    }
    
    delete[] B_trans;
    
    double end = getTime();
    return (double)(end-start);
}

