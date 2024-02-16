// A simple test executable for testing
// the COO loading functionality of the matrix
#include <iostream>

#include <spmm.h>
#include <ell/ell.h>

int main(int argc, char **argv) {
    ELL mtx(argc, argv);
    mtx.format();
    mtx.calculate();
    mtx.debug();
    
    std::cout << "---------------" << std::endl;
    
    ELL mtx2(argc, argv);
    mtx2.format();
    mtx2.benchmark();
    mtx2.debug();
    
    return 0;
}

