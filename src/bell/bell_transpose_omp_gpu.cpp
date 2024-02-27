#include <omp.h>

#include "matrix.h"

//
// The calculation algorithm for the current format
//
double Matrix::calculate() {
    double start = getTime();
    
    // Transpose
    double* B_trans = new double[rows * cols];

    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        B_trans[j * rows + i] = B[i*cols+j];
      }
    }
    
    uint64_t *_rowptr = rowptr;
    uint64_t *_colidx = colidx;
    double *_C = C;
    double *_values = values;
    size_t i = 0;
    size_t _rows = rows;
    size_t _cols = cols;
    size_t _k_bound = k_bound;
    
    #pragma omp target teams distribute parallel for \
        map(to: _rows, _cols, _k_bound, _rowptr[0:rows+1], _colidx[0:num_cols], _values[0:num_cols], B_trans[0:rows*cols]) \
        map(tofrom: _C[0:rows*cols])
    for (i = 0; i<rows; i++) {
        for (uint64_t p = _rowptr[i]; p<_rowptr[i+1]; p++) {
            uint64_t j = _colidx[p];
            uint64_t val = _values[p];
            for (uint64_t k = 0; k<_k_bound; k++) {
                _C[i*_cols+j] += val * B_trans[k*_cols+j];
            }
        }
    }
    
    C = _C;
    
    double end = getTime();
    return (double)(end-start);
}

