// A simple test executable for testing
// the COO loading functionality of the matrix
#include <iostream>
#include <string>
#include <cstdlib>

#include <spmm.h>
#include <csr.h>

__global__ void cuda_compute_csr(double *C, double *B, size_t rows, size_t cols, uint64_t *rowptr, uint64_t *rowidx, double *values) {
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < rows) {
        for (uint64_t p = rowptr[i]; p<rowptr[i+1]; p++) {
            uint64_t j = rowidx[p];
            for (uint64_t k = 0; k<rows; k++) {
                C[i*cols+k] += values[p] * B[j*cols+k];
            }
        }
    }
}

class CSR2 : public CSR {
public:
    explicit CSR2(std::string input) : CSR(input) {}

    double calculate() override {
        double start = getTime();
        
        double *dB, *dC, *dValues;
        uint64_t *dRowptr, *dRowidx;
        cudaMalloc((void**)&dB, sizeof(double)*(rows*cols));
        cudaMalloc((void**)&dC, sizeof(double)*(rows*cols));
        cudaMalloc((void**)&dValues, sizeof(double)*(coo->nnz));
        cudaMalloc((void**)&dRowptr, sizeof(uint64_t)*(rows+1));
        cudaMalloc((void**)&dRowidx, sizeof(uint64_t)*(coo->nnz));
        
        cudaMemcpy(dB, B, sizeof(double)*(rows*cols), cudaMemcpyHostToDevice);
        cudaMemcpy(dC, C, sizeof(double)*(rows*cols), cudaMemcpyHostToDevice);
        cudaMemcpy(dValues, values, sizeof(double)*(coo->nnz), cudaMemcpyHostToDevice);
        cudaMemcpy(dRowptr, rowptr, sizeof(uint64_t)*(rows+1), cudaMemcpyHostToDevice);
        cudaMemcpy(dRowidx, rowidx, sizeof(uint64_t)*(coo->nnz), cudaMemcpyHostToDevice);
        
        cuda_compute_csr<<<16,1>>>(dC, dB, rows, cols, dRowptr, dRowidx, dValues);
        
        cudaMemcpy(C, dC, sizeof(double)*(rows*cols), cudaMemcpyDeviceToHost);
        
        cudaFree(dB);
        cudaFree(dC);
        cudaFree(dValues);
        cudaFree(dRowptr);
        cudaFree(dRowidx);
        
        double end = getTime();
        return (double)(end-start);
    }
};

int main(int argc, char **argv) {
    CSR2 mtx(argv[1]);
    
    mtx.printSparse(false);
    std::cout << "-----------------" << std::endl;
    mtx.printDense(false);
    std::cout << "-----------------" << std::endl;
    double time = mtx.calculate();
    fprintf(stdout, "%lf\n", time);
    mtx.printResult(false);
    std::cout << "-----------------" << std::endl;
    
    return 0;
}

