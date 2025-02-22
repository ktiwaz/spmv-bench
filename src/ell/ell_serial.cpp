#include "matrix.h"

double _calculate(uint64_t rows, uint64_t num_cols, uint64_t *colidx, double *values, double *B, double *C) {
    double start = SpM::getTime2();
    
    // Iterate through rows of the matrix
    for (uint64_t i = 0; i < rows; i++) {
        double sum = 0.0;  // Initialize the sum for each row
        // Iterate through non-zero elements in the row
        for (uint64_t n1 = 0; n1 < num_cols; n1++) {
            uint64_t p = i * num_cols + n1;  // Index for non-zero element
            uint64_t j = colidx[p];          // Column index of the non-zero element
            double v = values[p];            // Value of the non-zero element
            sum += v * B[j];                 // Multiply and accumulate with vector B
        }
        C[i] = sum;  // Store the result in the output vector C
    }
    
    double end = SpM::getTime2();
    return (double)(end - start);
}

//
// The calculation algorithm for the current format
//
double Matrix::calculate() {
    return _calculate(rows, num_cols, colidx, values, B, C);
}

