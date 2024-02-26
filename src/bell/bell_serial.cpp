#include "matrix.h"

//
// The calculation algorithm for the current format
//
double Matrix::calculate() {
    double start = getTime();
    
    for (uint64_t i = 0; i<rows; i++) {
        for (uint64_t p = rowptr[i]; p<rowptr[i+1]; p++) {
            uint64_t j = colidx[p];
            uint64_t val = values[p];
            for (uint64_t k = 0; k<rows; k++) {
                C[i*cols+j] += val * B[k*cols+j];
            }
        }
    }
    
    double end = getTime();
    return (double)(end-start);
}

