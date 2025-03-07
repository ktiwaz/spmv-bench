#include "matrix.h"

double _calculate(uint64_t num_blocks, uint64_t block_rows, uint64_t block_cols,
                  uint64_t *colptr, uint64_t *colidx, double *values, double *B, double *C) {
    double start = SpM::getTime2();

    for (uint64_t n1 = 0; n1 < num_blocks; n1++) {
        for (uint64_t bi = 0; bi < block_rows; bi++) {
            for (uint64_t n2 = colptr[n1]; n2 < colptr[n1 + 1]; n2++) {
                for (uint64_t bj = 0; bj < block_cols; bj++) {
                    uint64_t i = n1 * block_rows + bi;  // Compute row index
                    uint64_t j = colidx[n2] * block_cols + bj;  // Compute column index
                    uint64_t index = n2 * (block_rows * block_cols) + bi * block_cols + bj;

                    C[i] += values[index] * B[j];  // SpMV computation
                }
            }
        }
    }

    double end = SpM::getTime2();
    return (double)(end - start);
}

double Matrix::calculate() {
    return _calculate(num_blocks, block_rows, block_cols, colptr, colidx, values, B, C);
}
