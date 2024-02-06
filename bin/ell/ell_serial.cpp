// A simple test executable for testing
// the COO loading functionality of the matrix
#include <iostream>
#include <string>
#include <cstdlib>

#include <spmm.h>
#include <ell.h>

class ELL2 : public ELL {
public:
    explicit ELL2(std::string input) : ELL(input) {}
    double calculate() override;
};

//
// The calculation algorithm for the current format
//
double ELL2::calculate() {
    double start = getTime();
    
    for (uint64_t i = 0; i<rows; i++) {
        for (uint64_t n1 = 0; n1<num_cols; n1++) {
            uint64_t p = i * num_cols + n1;
            uint64_t j = colidx[p];
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
    ELL2 mtx(argv[2]);
    
    std::cout << "ELL Serial -O2 -march=native,";
    mtx.benchmark(iters);
    
    return 0;
}

