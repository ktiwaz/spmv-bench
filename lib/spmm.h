#pragma once

#include <string>
#include <vector>
#include <cstdint>

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
class Matrix {
public:
    Matrix();
    COO *coo;
    
    double *generateDense();

protected:
    void initCOO(std::string input);
private:
    std::vector<std::string> split(std::string line);
};


