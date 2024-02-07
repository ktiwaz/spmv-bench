// A simple test executable for testing
// the COO loading functionality of the matrix
#include <iostream>
#include <string>
#include <cstdlib>

#include <spmm.h>
#include <bcsr.h>

class BCSR2 : public BCSR {
public:
    explicit BCSR2(std::string input, int block_row, int block_col) : BCSR(input, block_row, block_col) {}
    double calculate() override;
};

//
// The calculation algorithm for the current format
//
double BCSR2::calculate() {
    double start = getTime();
    
    for (uint64_t n1 = 0; n1<num_blocks; n1++) {
        for (uint64_t bi = 0; bi<block_rows; bi++) {
            for (uint64_t n2 = colptr[n1]; n2<colptr[n1+1]; n2++) {
                for (uint64_t bj = 0; bj<block_cols; bj++) {
                    for (uint64_t k = 0; k<rows; k++) {
                        uint64_t i = n1 * block_rows + bi;
                        uint64_t j = colidx[n2] * block_cols + bj;
                        uint64_t index = n2*(block_rows*block_cols) + bi * block_cols + bj;
                        
                        C[i*cols+k] += values[index] * B[i*cols+j];
                    }
                }
            }
        }
    }
    
    double end = getTime();
    return (double)(end-start);
}

int main(int argc, char **argv) {
    int iters = std::stoi(argv[1]);
    int block_row = std::stoi(argv[3]);
    int block_col = std::stoi(argv[4]);
    BCSR2 mtx(argv[2], block_row, block_col);
    
    std::cout << "BCSR Serial -O2 -march=native " << block_row << "x" << block_col << ",";
    mtx.benchmark(iters);
    
    return 0;
}

