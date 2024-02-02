// A simple test executable for testing
// the COO loading functionality of the matrix
#include <iostream>

#include <spmm.h>

int main(int argc, char **argv) {
    Matrix mtx;
    COO *coo = mtx.coo;
    
    std::cout << "Rows: " << coo->rows << " | Cols: " << coo->cols << " | NNZ: " << coo->nnz << std::endl;
    std::cout << "----------" << std::endl;
    for (auto item : coo->items) {
        std::cout << item.row << ", " << item.col << " = " << item.val << std::endl;
    }
    
    return 0;
}

