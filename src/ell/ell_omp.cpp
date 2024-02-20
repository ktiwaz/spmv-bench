#include <omp.h>

#include "matrix.h"

//
// The calculation algorithm for the current format
//
double Matrix::calculate() {
    if (threads != -1) omp_set_num_threads(threads);
    
    double start = getTime();
    
    #pragma omp parallel for
    for (uint64_t i = 0; i<rows; i++) {
        for (uint64_t n1 = 0; n1<num_cols; n1++) {
            uint64_t p = i * num_cols + n1;
            uint64_t j = colidx[p];
            #pragma omp simd
            for (uint64_t k = 0; k<rows; k++) {
                C[i*cols+j] += values[p] * B[k*cols+j];
            }
        }
    }
    
    double end = getTime();
    return (double)(end-start);
}

