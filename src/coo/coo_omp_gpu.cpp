#include "matrix.h"

template<uint64_t K>
double _calculate(size_t nnz, uint64_t *coo_rows, uint64_t *coo_cols, double *coo_vals, double *C, double *B, uint64_t cols, uint64_t rows) {
    double start = SpM::getTime2();
    
    double *_B = B;
    double *_C = C;
    double *_coo_vals = coo_vals;
    size_t arg0 = 0;
    size_t _cols = cols;
    uint64_t *_coo_rows = coo_rows;
    uint64_t *_coo_cols = coo_cols;
    
    #pragma omp target teams distribute parallel for \
        map(to: _cols, nnz, _coo_rows[0:nnz], _coo_cols[0:nnz], _coo_vals[0:nnz], _B[0:rows*cols]) \
        map(tofrom: _C[0:rows*cols])
    for (arg0 = 0; arg0<nnz; arg0++) {
        size_t _arg0 = arg0;
        uint64_t i = _coo_rows[_arg0];
        uint64_t j = _coo_cols[_arg0];
        double val = _coo_vals[_arg0];
        for (size_t k = 0; k<K; k++) {
            _C[i*_cols+k] += val * _B[j*_cols+k];
        }
    }
    
    C = _C;
    
    double end = SpM::getTime2();
    return (double)(end-start);
}

double Matrix::calculate() {
    switch (k_bound) {
        case 16: return _calculate<16>(coo->nnz, coo_rows, coo_cols, coo_vals, C, B, cols, rows);
        case 64: return _calculate<64>(coo->nnz, coo_rows, coo_cols, coo_vals, C, B, cols, rows);
        case 128: return _calculate<128>(coo->nnz, coo_rows, coo_cols, coo_vals, C, B, cols, rows);
        default: return 0;
    }
}


