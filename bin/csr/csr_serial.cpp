// A simple test executable for testing
// the COO loading functionality of the matrix
#include <iostream>
#include <string>
#include <cstdlib>

#include <spmm.h>
#include <csr.h>

class CSR2 : public CSR {
public:
    explicit CSR2(std::string input) : CSR(input) {}
    double calculate() override;
};

//
// The calculation algorithm for the current format
//
double CSR2::calculate() {
    double start = getTime();
    
    for (size_t i = 0; i<rows; i++) {
        for (uint64_t p = rowptr[i]; p<rowptr[i+1]; p++) {
            uint64_t j = rowidx[p];
            for (uint64_t k = 0; k<rows; k++) {
                C[i*cols+k] += values[p] * B[j*cols+k];
            }
        }
    }
    
    double end = getTime();
    return (double)(end-start);
}

int main(int argc, char **argv) {
    int iters = std::stoi(argv[1]);
    CSR2 mtx(argv[2]);
    
    std::cout << "CSR Serial -O2 -march=native,";
    mtx.benchmark(iters);
    
    return 0;
}

