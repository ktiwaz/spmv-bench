#include <omp.h>

#include "matrix.h"

//
// The calculation algorithm for the current format
//
double Matrix::calculate() {
    double start = getTime();
    
    uint64_t *_rowptr = rowptr;
    uint64_t *_colidx = colidx;
    double *_B = B;
    double *_C = C;
    double *_values = values;
    size_t i = 0;
    size_t _rows = rows;
    size_t _cols = cols;
    
    #pragma omp target teams distribute parallel for \
        map(to: _rows, _cols, _rowptr[0:rows+1], _colidx[0:num_cols], _values[0:num_cols], _B[0:rows*cols]) \
        map(tofrom: _C[0:rows*cols])
    for (i = 0; i<rows; i++) {
        for (uint64_t p = _rowptr[i]; p<_rowptr[i+1]; p++) {
            uint64_t j = _colidx[p];
            uint64_t val = _values[p];
            for (uint64_t k = 0; k<_cols; k++) {
                _C[i*_cols+j] += val * _B[k*_cols+j];
            }
        }
    }
    
    C = _C;
    
    double end = getTime();
    return (double)(end-start);
}

