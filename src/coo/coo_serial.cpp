#include "matrix.h"

double Matrix::calculate() {
    double start = SpM::getTime2();
    
    // Standard SpMV: y = Ax
    for (size_t arg0 = 0; arg0 < coo->nnz; arg0++) {
        size_t i = coo_rows[arg0]; // Row index
        size_t j = coo_cols[arg0]; // Column index
        double val = coo_vals[arg0]; // Nonzero value
        
        C[i] += val * B[j]; // SpMV operation (C = y, B = x)
    }
    
    double end = SpM::getTime2();
    return (double)(end - start);
}

