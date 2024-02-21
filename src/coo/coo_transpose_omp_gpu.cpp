#include "matrix.h"

double Matrix::calculate() {
    double start = getTime();
    
    // Transpose
    double* B_trans = new double[rows * cols];

    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        B_trans[j * rows + i] = B[i*cols+j];
      }
    }
    
    double *_B = B;
    double *_C = C;
    double *_coo_vals = coo_vals;
    size_t arg0 = 0;
    size_t _cols = cols;
    size_t nnz = coo->nnz;
    uint64_t *_coo_rows = coo_rows;
    uint64_t *_coo_cols = coo_cols;
    
    #pragma omp target teams distribute parallel for \
        map(to: _cols, nnz, _coo_rows[0:nnz], _coo_cols[0:nnz], _coo_vals[0:nnz], B_trans[0:rows*cols]) \
        map(tofrom: _C[0:rows*cols])
    for (arg0 = 0; arg0<nnz; arg0++) {
        size_t _arg0 = arg0;
        uint64_t i = _coo_rows[_arg0];
        uint64_t j = _coo_cols[_arg0];
        double val = _coo_vals[_arg0];
        for (size_t k = 0; k<_cols; k++) {
            _C[i*_cols+k] += val * B_trans[j*_cols+k];
        }
    }
    
    C = _C;
    
    double end = getTime();
    return (double)(end-start);
}

