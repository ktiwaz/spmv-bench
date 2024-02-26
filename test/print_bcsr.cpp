// A simple test executable for testing
// the COO loading functionality of the matrix
#include <iostream>
#include <string>
#include <cstdlib>

#include <spmm.h>
#include <bcsr/bcsr.h>

int main(int argc, char **argv) {
    BCSR mtx(argc, argv);
    mtx.format();
    mtx.calculate();
    mtx.verify();
    mtx.debug();
    
    std::cout << "---------------" << std::endl;
    
    BCSR mtx2(argc, argv);
    mtx2.format();
    mtx2.benchmark();
    mtx2.verify();
    mtx2.debug();
    
    return 0;
}

