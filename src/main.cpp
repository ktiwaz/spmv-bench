#include "matrix.h"

int main(int argc, char **argv) {
    Matrix mtx(argc, argv);
    mtx.format();
    mtx.benchmark();
    mtx.verify();
    mtx.report();
    
    return 0;
}

