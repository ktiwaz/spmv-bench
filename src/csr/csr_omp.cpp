#include "matrix.h"

template<uint64_t K>
double _calculate(size_t rows, uint64_t *rowptr, uint64_t *rowidx, double *values, double *C, double *B, uint64_t cols) {
    double start = SpM::getTime2();
    
    #pragma omp parallel for
    for (size_t i = 0; i<rows; i++) {
        for (uint64_t p = rowptr[i]; p<rowptr[i+1]; p++) {
            uint64_t j = rowidx[p];
            double v = values[p];
            for (uint64_t k = 0; k<K; k++) {
                C[i*cols+k] += v * B[j*cols+k];
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
        case 16: return _calculate<16>(rows, rowptr, rowidx, values, C, B, cols);
        case 64: return _calculate<64>(rows, rowptr, rowidx, values, C, B, cols);
        case 128: return _calculate<128>(rows, rowptr, rowidx, values, C, B, cols);
        default: return 0;
    }
}

