#include <omp.h>

#include "matrix.h"

template<uint64_t K>
double _calculate(uint64_t num_blocks, uint64_t block_rows, uint64_t block_cols,
                  uint64_t *colptr, uint64_t *colidx, double *values, double *B, double *C, uint64_t cols, uint64_t rows,
                  uint64_t colptr_len, uint64_t colidx_len, uint64_t value_len) {
    double start = SpM::getTime2();
    
    size_t _rows = rows;
    size_t _cols = cols;
    size_t _block_rows = block_rows;
    size_t _block_cols = block_cols;
    uint64_t *_colptr = colptr;
    uint64_t *_colidx = colidx;
    double *_values = values;
    double *_B = B;
    double *_C = C;
    uint64_t n1 = 0;
    
    #pragma omp target teams distribute parallel for \
        map(to: _rows, _cols, _block_rows, _block_cols, _colptr[0:colptr_len], _colidx[0:colidx_len]) \
        map(to: _values[0:value_len], _B[0:rows*cols]) \
        map(tofrom: _C[0:rows*cols])
    for (n1 = 0; n1<num_blocks; n1++) {
        uint64_t _n1 = n1;
        for (uint64_t bi = 0; bi<_block_rows; bi++) {
            for (uint64_t n2 = _colptr[n1]; n2<_colptr[n1+1]; n2++) {
                for (uint64_t bj = 0; bj<_block_cols; bj++) {
                    uint64_t i = n1 * _block_rows + bi;
                    uint64_t j = _colidx[n2] * _block_cols + bj;
                    uint64_t index = n2*(_block_rows*_block_cols) + bi * _block_cols + bj;
                    double v = _values[index];
                    
                    for (uint64_t k = 0; k<K; k++) {
                        _C[i*_cols+k] += v * _B[j*_cols+k];
                    }
                }
            }
        }
    }
    
    double end = SpM::getTime2();
    return (double)(end-start);
}

//
// The calculation algorithm for the current format
//
double Matrix::calculate() {
    switch (k_bound) {
        case 16: return _calculate<16>(num_blocks, block_rows, block_cols, colptr, colidx, values, B, C, cols, rows, colptr_len, colidx_len, value_len);
        case 64: return _calculate<64>(num_blocks, block_rows, block_cols, colptr, colidx, values, B, C, cols, rows, colptr_len, colidx_len, value_len);
        case 128: return _calculate<128>(num_blocks, block_rows, block_cols, colptr, colidx, values, B, C, cols, rows, colptr_len, colidx_len, value_len);
        default: return 0;
    }
}

