#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <iostream>

struct COOItem {
    size_t row;
    size_t col;
    double val;
};

struct COO {
    size_t nnz;
    size_t rows;
    size_t cols;
    std::vector<COOItem> items;
};

//
// The base class for loading and formatting a matrix
//
class SpM {
public:
    SpM();
    void printDense();
    void printResult();
    
    //
    // The print method for the current sparse matrix
    //
    virtual void printSparse() {
        std::cout << "Rows: " << coo->rows << " | Cols: " << coo->cols << " | NNZ: " << coo->nnz << std::endl;
        std::cout << "----------" << std::endl;
        
        std::cout << "Rows: ";
        for (auto item : coo->items) std::cout << item.row << ",";
        std::cout << std::endl;
        
        std::cout << "Cols: ";
        for (auto item : coo->items) std::cout << item.col << ",";
        std::cout << std::endl;
        
        std::cout << "Vals: ";
        for (auto item : coo->items) std::cout << item.val << ",";
        std::cout << std::endl;
    }
    
    //
    // The calculation algorithm for the current format
    //
    virtual void calculate() {
        for (size_t arg0 = 0; arg0<coo->nnz; arg0++) {
            auto item = coo->items[arg0];
            size_t i = item.row;
            size_t j = item.col;
            double val = item.val;
            for (size_t k = 0; k<rows; k++) {
                C[i*cols+k] += val * B[j*cols+k];
            }
        }
    }
    
protected:
    int rows, cols;
    COO *coo;
    double *B;
    double *C;
    
    void initCOO(std::string input);
    void generateDense();
private:
    std::vector<std::string> split(std::string line);
};


