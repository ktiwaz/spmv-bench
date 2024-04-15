#include "matrix.h"

template<uint64_t K>
double _calculate(uint64_t num_blocks, uint64_t block_rows, uint64_t block_cols,
                  uint64_t *colptr, uint64_t *colidx, double *values, double *B, double *C, uint64_t cols) {
    double start = SpM::getTime2();
    
    for (uint64_t n1 = 0; n1<num_blocks; n1++) {
        for (uint64_t bi = 0; bi<block_rows; bi++) {
            for (uint64_t n2 = colptr[n1]; n2<colptr[n1+1]; n2++) {
                for (uint64_t bj = 0; bj<block_cols; bj++) {
                    uint64_t i = n1 * block_rows + bi;
                    uint64_t j = colidx[n2] * block_cols + bj;
                    uint64_t index = n2*(block_rows*block_cols) + bi * block_cols + bj;
                    double v = values[index];
                    
                    for (uint64_t k = 0; k<K; k++) {
                        C[i*cols+k] += v * B[j*cols+k];
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
        case 16: return _calculate<16>(num_blocks, block_rows, block_cols, colptr, colidx, values, B, C, cols);
        case 64: return _calculate<64>(num_blocks, block_rows, block_cols, colptr, colidx, values, B, C, cols);
        case 128: return _calculate<128>(num_blocks, block_rows, block_cols, colptr, colidx, values, B, C, cols);
        default: return 0;
    }
}


