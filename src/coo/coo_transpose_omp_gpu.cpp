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
    double start = getTime();
    
    transpose(B, cols);
    
    double *_B = B;
    double *_C = C;
    double *_coo_vals = coo_vals;
    size_t arg0 = 0;
    size_t _cols = cols;
    size_t nnz = coo->nnz;
    size_t _k_bound = k_bound;
    uint64_t *_coo_rows = coo_rows;
    uint64_t *_coo_cols = coo_cols;
    
    #pragma omp target teams distribute parallel for \
        map(to: _cols, nnz, _k_bound, _coo_rows[0:nnz], _coo_cols[0:nnz], _coo_vals[0:nnz], B[0:rows*cols]) \
        map(tofrom: _C[0:rows*cols])
    for (arg0 = 0; arg0<nnz; arg0++) {
        size_t _arg0 = arg0;
        uint64_t i = _coo_rows[_arg0];
        uint64_t j = _coo_cols[_arg0];
        double val = _coo_vals[_arg0];
        for (size_t k = 0; k<_k_bound; k++) {
            _C[i*_cols+k] += val * B[k*_cols+j];
        }
    }
    
    C = _C;
    
    double end = getTime();
    transpose(B, cols);
    return (double)(end-start);
}

