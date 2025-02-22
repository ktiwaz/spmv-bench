#include "matrix.h"

double Matrix::calculate() {
    double start = SpM::getTime2();
    
    // Perform SpMV using OpenMP for parallelization
    #pragma omp parallel for
    for (size_t arg0 = 0; arg0 < coo->nnz; arg0++) {
        size_t i = coo_rows[arg0];
        size_t j = coo_cols[arg0];
        double val = coo_vals[arg0];

        // For SpMV, we perform a dot product for each row of A with the vector B
        // C[i] += A(i, j) * B[j], where A(i, j) is coo_vals[arg0] and B[j] is the vector
        C[i] += val * B[j];  // no need for a loop over k
    }
    
    double end = SpM::getTime2();
    return (double)(end - start);
}
