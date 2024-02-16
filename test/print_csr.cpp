// A simple test executable for testing
// the COO loading functionality of the matrix
#include <iostream>

#include <spmm.h>
#include <csr/csr.h>

int main(int argc, char **argv) {
    CSR mtx(argc, argv);
    mtx.format();
    mtx.calculate();
    mtx.debug();
    
    std::cout << "---------------" << std::endl;
    
    CSR mtx2(argc, argv);
    mtx2.format();
    mtx2.benchmark();
    mtx2.debug();
    
    return 0;
}

