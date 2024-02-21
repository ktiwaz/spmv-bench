#include <omp.h>

#include "matrix.h"

//
// The calculation algorithm for the current format
//
double Matrix::calculate() {
    if (threads != -1) omp_set_num_threads(threads);
    
    double start = getTime();
    
    // Transpose
    double* B_trans = new double[rows * cols];

    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        B_trans[j * rows + i] = B[i*cols+j];
      }
    }
    
    #pragma omp parallel for
    for (uint64_t n1 = 0; n1<num_blocks; n1++) {
        for (uint64_t bi = 0; bi<block_rows; bi++) {
            for (uint64_t n2 = colptr[n1]; n2<colptr[n1+1]; n2++) {
                for (uint64_t bj = 0; bj<block_cols; bj++) {
                    for (uint64_t k = 0; k<rows; k++) {
                        uint64_t i = n1 * block_rows + bi;
                        uint64_t j = colidx[n2] * block_cols + bj;
                        uint64_t index = n2*(block_rows*block_cols) + bi * block_cols + bj;
                        
                        C[i*cols+j] += values[index] * B_trans[j*cols+k];
                    }
                }
            }
        }
    }
    
    double end = getTime();
    return (double)(end-start);
}

