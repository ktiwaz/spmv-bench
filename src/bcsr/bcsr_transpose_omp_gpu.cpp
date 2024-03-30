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
    
    size_t _rows = rows;
    size_t _cols = cols;
    size_t _block_rows = block_rows;
    size_t _block_cols = block_cols;
    size_t _k_bound = k_bound;
    uint64_t *_colptr = colptr;
    uint64_t *_colidx = colidx;
    double *_values = values;
    double *_C = C;
    uint64_t n1 = 0;
    
    #pragma omp target teams distribute parallel for \
        map(to: _rows, _cols, _block_rows, _block_cols, _k_bound, _colptr[0:colptr_len], _colidx[0:colidx_len]) \
        map(to: _values[0:value_len], B[0:rows*cols]) \
        map(tofrom: _C[0:rows*cols])
    for (n1 = 0; n1<num_blocks; n1++) {
        uint64_t _n1 = n1;
        for (uint64_t bi = 0; bi<_block_rows; bi++) {
            for (uint64_t n2 = _colptr[n1]; n2<_colptr[n1+1]; n2++) {
                for (uint64_t bj = 0; bj<_block_cols; bj++) {
                    for (uint64_t k = 0; k<_k_bound; k++) {
                        uint64_t i = n1 * _block_rows + bi;
                        uint64_t j = _colidx[n2] * _block_cols + bj;
                        uint64_t index = n2*(_block_rows*_block_cols) + bi * _block_cols + bj;
                        
                        _C[i*_cols+k] += _values[index] * B[k*_cols+j];
                    }
                }
            }
        }
    }
    
    C = _C;
    
    double end = getTime();
    transpose(B, cols);
    return (double)(end-start);
}

