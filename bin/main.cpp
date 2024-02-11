#include "matrix.h"

int main(int argc, char **argv) {
    Matrix mtx(argc, argv);
    mtx.format();
    mtx.benchmark();
    
    return 0;
}

