// A simple test executable for testing
// the COO loading functionality of the matrix
#include <iostream>

#include <spmm.h>
#include <bcsr.h>

int main(int argc, char **argv) {
    BCSR mtx("../test_rank2.mtx", 2, 2);
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

