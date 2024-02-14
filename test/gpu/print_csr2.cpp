// A simple test executable for testing
// the COO loading functionality of the matrix
#include <iostream>
#include <string>
#include <cstdlib>

#include <spmm.h>
#include <csr.h>

class CSR2 : public CSR {
public:
    explicit CSR2(int argc, char **argv) : CSR(argc, argv) {}

    double calculate() override {
        double start = getTime();
        
        uint64_t *_rowptr = rowptr;
        uint64_t *_rowidx = rowidx;
        double *_B = B;
        double *_C = C;
        double *_values = values;
        size_t i = 0;
        size_t _rows = rows;
        size_t _cols = cols;
        
        //#pragma omp target parallel for map(to: _rowptr[0:rows+1], _rowidx[0:rowptr[rows]], _values[0:_rowptr[rows]]) \
        //    map(tofrom: _C[0:rows*cols]) map(_B[0:rows*cols])
        #pragma omp target teams distribute parallel for \
            map(tofrom: _rows, _cols, _rowptr[0:rows+1], _rowidx[0:coo->nnz], _values[0:coo->nnz], _B[0:rows*cols]) \
            map(tofrom: _C[0:rows*cols])
        for (i = 0; i < rows; i++) {
            size_t _i = i;
            for (uint64_t p = _rowptr[_i]; p < _rowptr[_i+1]; p++) {
                uint64_t j = _rowidx[p];
                for (uint64_t k = 0; k < cols; k++) {
                    _C[i * _cols + k] += _values[p] * _B[j * _cols + k];
                }
            }
        }
        
        C = _C;
        
        double end = getTime();
        return (double)(end-start);
    }
};

int main(int argc, char **argv) {
    CSR2 mtx(argc, argv);
    mtx.format();
    mtx.calculate();
    mtx.verify();
    mtx.debug();
    
    return 0;
}

