// A simple test executable for testing
// the COO loading functionality of the matrix
#include <iostream>

#include <spmm.h>
#include <bell/bell.h>

int main(int argc, char **argv) {
    BELL mtx(argc, argv);
    mtx.format();
    mtx.calculate();
    mtx.verify();
    mtx.debug();
    
    /*std::cout << "---------------" << std::endl;
    
    BELL mtx2(argc, argv);
    mtx2.format();
    mtx2.benchmark();
    mtx2.debug();*/
    
    return 0;
}

