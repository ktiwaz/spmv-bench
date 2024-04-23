#include "matrix.h"

template<uint64_t K>
double _calculate(uint64_t rows, uint64_t cols, uint64_t num_cols, uint64_t *colidx, double *values, double *B, double *C) {
    double start = SpM::getTime2();
    
    #pragma omp parallel for
    for (uint64_t i = 0; i<rows; i++) {
        for (uint64_t n1 = 0; n1<num_cols; n1++) {
            uint64_t p = i * num_cols + n1;
            uint64_t j = colidx[p];
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
        case 8: return _calculate<8>(rows, cols, num_cols, colidx, values, B, C);
        case 16: return _calculate<16>(rows, cols, num_cols, colidx, values, B, C);
        case 64: return _calculate<64>(rows, cols, num_cols, colidx, values, B, C);
        case 128: return _calculate<128>(rows, cols, num_cols, colidx, values, B, C);
        case 256: return _calculate<256>(rows, cols, num_cols, colidx, values, B, C);
        case 512: return _calculate<512>(rows, cols, num_cols, colidx, values, B, C);
        case 1028: return _calculate<1028>(rows, cols, num_cols, colidx, values, B, C);
        default: return 0;
    }
}

