#include <omp.h>

#include "matrix.h"

template<uint64_t K>
double _calculate(size_t rows, uint64_t *rowptr, uint64_t *rowidx, double *values, double *C, double *B, uint64_t cols, uint64_t nnz) {
    double start = SpM::getTime2();
    
    uint64_t *_rowptr = rowptr;
    uint64_t *_rowidx = rowidx;
    double *_B = B;
    double *_C = C;
    double *_values = values;
    size_t i = 0;
    size_t _rows = rows;
    size_t _cols = cols;
    
    #pragma omp target teams distribute parallel for \
        map(to: _rows, _cols, _rowptr[0:rows+1], _rowidx[0:nnz], _values[0:nnz], _B[0:rows*cols]) \
        map(tofrom: _C[0:rows*cols])
    for (i = 0; i < rows; i++) {
        size_t _i = i;
        for (uint64_t p = _rowptr[_i]; p < _rowptr[_i+1]; p++) {
            uint64_t j = _rowidx[p];
            double v = _values[p];
            for (uint64_t k = 0; k < K; k++) {
                _C[i * _cols + k] += v * _B[j * _cols + k];
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
        case 16: return _calculate<16>(rows, rowptr, rowidx, values, C, B, cols, coo->nnz);
        case 64: return _calculate<64>(rows, rowptr, rowidx, values, C, B, cols, coo->nnz);
        case 128: return _calculate<128>(rows, rowptr, rowidx, values, C, B, cols, coo->nnz);
        default: return 0;
    }
}


