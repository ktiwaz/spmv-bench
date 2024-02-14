#include "matrix.h"

double Matrix::calculate() {
    double start = getTime();
    
    for (size_t arg0 = 0; arg0<coo->nnz; arg0++) {
        auto item = coo->items[arg0];
        size_t i = item.row;
        size_t j = item.col;
        double val = item.val;
        for (size_t k = 0; k<cols; k++) {
            C[i*cols+k] += val * B[j*cols+k];
        }
    }
    
    double end = getTime();
    return (double)(end-start);
}

