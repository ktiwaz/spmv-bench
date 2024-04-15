#include "matrix.h"

template<uint64_t K>
double _calculate(size_t nnz, uint64_t *coo_rows, uint64_t *coo_cols, double *coo_vals, double *C, double *B, uint64_t cols) {
    double start = SpM::getTime2();
    
    for (size_t arg0 = 0; arg0<nnz; arg0++) {
        size_t i = coo_rows[arg0];
        size_t j = coo_cols[arg0];
        double val = coo_vals[arg0];
        for (size_t k = 0; k<K; k++) {
            C[i*cols+k] += val * B[j*cols+k];
        }
    }
    
    double end = SpM::getTime2();
    return (double)(end-start);
}

double Matrix::calculate() {
    switch (k_bound) {
        case 16: return _calculate<16>(coo->nnz, coo_rows, coo_cols, coo_vals, C, B, cols);
        case 64: return _calculate<64>(coo->nnz, coo_rows, coo_cols, coo_vals, C, B, cols);
        case 128: return _calculate<128>(coo->nnz, coo_rows, coo_cols, coo_vals, C, B, cols);
        default: return 0;
    }
}

