// A simple test executable for testing
// the COO loading functionality of the matrix
#include <iostream>

#include <spmm.h>
#include <csr.h>

int main(int argc, char **argv) {
    CSR mtx("../test_rank2.mtx");
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

