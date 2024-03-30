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
    
    size_t i = 0;
    size_t _rows = rows;
    size_t _cols = cols;
    size_t _num_cols = num_cols;
    size_t _k_bound = k_bound;
    uint64_t *_colidx = colidx;
    double *_values = values;
    double *_C = C;
    
    #pragma omp target teams distribute parallel for \
        map(to: _rows, _cols, _num_cols, _k_bound, _colidx[0:rows*num_cols], _values[0:rows*num_cols], B[0:rows*cols]) \
        map(tofrom: _C[0:rows*cols])
    for (i = 0; i<rows; i++) {
        size_t _i = i;
        for (uint64_t n1 = 0; n1<_num_cols; n1++) {
            uint64_t p = _i * _num_cols + n1;
            uint64_t j = _colidx[p];
            for (uint64_t k = 0; k<_k_bound; k++) {
                _C[_i*_cols+k] += _values[p] * B[k*_cols+j];
            }
        }
    }
    
    C = _C;
    
    double end = getTime();
    transpose(B, cols);
    return (double)(end-start);
}

