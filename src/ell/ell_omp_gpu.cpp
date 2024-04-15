#include <omp.h>

#include "matrix.h"

template<uint64_t K>
double _calculate(uint64_t rows, uint64_t cols, uint64_t num_cols, uint64_t *colidx, double *values, double *B, double *C) {
    double start = SpM::getTime2();
    
    size_t i = 0;
    size_t _rows = rows;
    size_t _cols = cols;
    size_t _num_cols = num_cols;
    uint64_t *_colidx = colidx;
    double *_values = values;
    double *_B = B;
    double *_C = C;
    
    #pragma omp target teams distribute parallel for \
        map(to: _rows, _cols, _num_cols, _colidx[0:rows*num_cols], _values[0:rows*num_cols], _B[0:rows*cols]) \
        map(tofrom: _C[0:rows*cols])
    for (i = 0; i<rows; i++) {
        size_t _i = i;
        for (uint64_t n1 = 0; n1<_num_cols; n1++) {
            uint64_t p = _i * _num_cols + n1;
            uint64_t j = _colidx[p];
            double v = _values[p];
            
            for (uint64_t k = 0; k<K; k++) {
                _C[_i*_cols+k] += v * _B[j*_cols+k];
            }
        }
    }
    
    C = _C;
    
    double end = SpM::getTime2();
    return (double)(end-start);
}

//
// The calculation algorithm for the current format
//
double Matrix::calculate() {
    switch (k_bound) {
        case 16: return _calculate<16>(rows, cols, num_cols, colidx, values, B, C);
        case 64: return _calculate<64>(rows, cols, num_cols, colidx, values, B, C);
        case 128: return _calculate<128>(rows, cols, num_cols, colidx, values, B, C);
        default: return 0;
    }
}


