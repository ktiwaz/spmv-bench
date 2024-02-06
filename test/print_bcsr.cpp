// A simple test executable for testing
// the COO loading functionality of the matrix
#include <iostream>
#include <string>
#include <cstdlib>

#include <spmm.h>
#include <bcsr.h>

int main(int argc, char **argv) {
    std::string input = std::string(argv[1]);
    int block_row = std::stoi(argv[2]);
    int block_col = std::stoi(argv[3]);
    BCSR mtx(input, block_row, block_col);
    
    mtx.printSparse();
    std::cout << "-----------------" << std::endl;
    mtx.printDense();
    std::cout << "-----------------" << std::endl;
    mtx.calculate();
    mtx.printResult();
    std::cout << "-----------------" << std::endl;
    
    mtx.benchmark(100);
    
    return 0;
}

