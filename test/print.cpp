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
    
    std::cout << "==========" << std::endl;
    double *mtx1 = mtx.generateDense();
    for (size_t i = 0; i<coo->rows; i++) {
        for (size_t j = 0; j<coo->cols; j++) {
            std::cout << mtx1[i*coo->cols+j] << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}

