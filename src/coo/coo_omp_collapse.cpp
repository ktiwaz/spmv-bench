#include "matrix.h"

double Matrix::calculate() {
    double start = getTime();
    
    #pragma omp parallel for collapse(1)
    for (size_t arg0 = 0; arg0<coo->nnz; arg0++) {
        size_t i = coo_rows[arg0];
        size_t j = coo_cols[arg0];
        double val = coo_vals[arg0];
        for (size_t k = 0; k<k_bound; k++) {
            C[i*cols+k] += val * B[j*cols+k];
        }
    }
    
    double end = getTime();
    return (double)(end-start);
}

