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
    uint64_t *_rowidx = rowidx;
    double *_C = C;
    double *_values = values;
    size_t i = 0;
    size_t _rows = rows;
    size_t _cols = cols;
    size_t _k_bound = k_bound;
    
    #pragma omp target teams distribute parallel for \
        map(to: _rows, _cols, _k_bound, _rowptr[0:rows+1], _rowidx[0:coo->nnz], _values[0:coo->nnz], B_trans[0:rows*cols]) \
        map(tofrom: _C[0:rows*cols])
    for (i = 0; i < rows; i++) {
        size_t _i = i;
        for (uint64_t p = _rowptr[_i]; p < _rowptr[_i+1]; p++) {
            uint64_t j = _rowidx[p];
            for (uint64_t k = 0; k < _k_bound; k++) {
                _C[i * _cols + j] += _values[p] * B_trans[j * _cols + k];
            }
        }
    }
    
    C = _C;
    
    double end = getTime();
    return (double)(end-start);
}

