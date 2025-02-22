#include "matrix.h"

// Perform the SpMV calculation
template<uint64_t K>
double _calculate(size_t rows, uint64_t *rowptr, uint64_t *rowidx, double *values, double *C, double *B, uint64_t cols) {
    double start = SpM::getTime2();
    
    // Parallelize the outer loop using OpenMP
    #pragma omp parallel for
    for (size_t i = 0; i < rows; i++) {
        double sum = 0.0; // Initialize the result for the row
        for (uint64_t p = rowptr[i]; p < rowptr[i + 1]; p++) {
            uint64_t j = rowidx[p];
            double v = values[p];
            // Instead of iterating over K, we just multiply with the vector B
            sum += v * B[j]; // Perform the dot product for the sparse row and the dense vector
        }
        C[i] = sum; // Store the result in the output vector C
    }

    double end = SpM::getTime2();
    return (double)(end - start);
}

// The calculation algorithm for the current format
double Matrix::calculate() {
    // Only one case for SpMV as we don't need k_bound anymore
    return _calculate<1>(rows, rowptr, rowidx, values, C, B, cols);
}
