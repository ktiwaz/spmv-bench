#include <omp.h>

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
    
    uint64_t *_rowptr = rowptr;
    uint64_t *_rowidx = rowidx;
    double *_C = C;
    double *_values = values;
    size_t i = 0;
    size_t _rows = rows;
    size_t _cols = cols;
    size_t _k_bound = k_bound;
    
    #pragma omp target teams distribute parallel for \
        map(to: _rows, _cols, _k_bound, _rowptr[0:rows+1], _rowidx[0:coo->nnz], _values[0:coo->nnz], B[0:rows*cols]) \
        map(tofrom: _C[0:rows*cols])
    for (i = 0; i < rows; i++) {
        size_t _i = i;
        for (uint64_t p = _rowptr[_i]; p < _rowptr[_i+1]; p++) {
            uint64_t j = _rowidx[p];
            for (uint64_t k = 0; k < _k_bound; k++) {
                _C[i * _cols + k] += _values[p] * B[k * _cols + j];
            }
        }
    }
    
    C = _C;
    
    double end = getTime();
    transpose(B, cols);
    return (double)(end-start);
}

