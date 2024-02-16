// A simple test executable for testing
// the COO loading functionality of the matrix
#include <iostream>
#include <string>
#include <cstdlib>

#include <spmm.h>
#include <ell/ell.h>

class ELL2 : public ELL {
public:
    explicit ELL2(int argc, char **argv) : ELL(argc, argv) {}

    double calculate() override {
        double start = getTime();
        
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
                for (uint64_t k = 0; k<rows; k++) {
                    _C[_i*_cols+k] += _values[p] * _B[j*_cols+k];
                }
            }
        }
        
        C = _C;
        
        double end = getTime();
        return (double)(end-start);
    }
};

int main(int argc, char **argv) {
    ELL2 mtx(argc, argv);
    mtx.format();
    mtx.calculate();
    mtx.verify();
    mtx.debug();
    
    return 0;
}

