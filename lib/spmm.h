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
    SpM(std::string input);
    void printDense(bool all = true);
    void printResult(bool all = true);
    void benchmark(size_t iters);
    
    //
    // The format method
    // Called for formats that inherit this base class
    //
    virtual void format() {
        std::cout << "COO- No formatting needed" << std::endl;
    }
    
    //
    // The print method for the current sparse matrix
    //
    virtual void printSparse(bool all = true) {
        std::cout << "Rows: " << coo->rows << " | Cols: " << coo->cols << " | NNZ: " << coo->nnz << std::endl;
        std::cout << "----------" << std::endl;
        
        std::cout << "Rows: ";
        for (size_t i = 0; i<coo->items.size(); i++) {
            auto item = coo->items[i];
            std::cout << item.row << ",";
            if (i == 20 && !all) break;
        }
        std::cout << std::endl;
        
        std::cout << "Cols: ";
        for (size_t i = 0; i<coo->items.size(); i++) {
            auto item = coo->items[i];
            std::cout << item.col << ",";
            if (i == 20 && !all) break;
        }
        std::cout << std::endl;
        
        std::cout << "Vals: ";
        for (size_t i = 0; i<coo->items.size(); i++) {
            auto item = coo->items[i];
            std::cout << item.val << ",";
            if (i == 20 && !all) break;
        }
        std::cout << std::endl;
    }
    
    //
    // The calculation algorithm for the current format
    //
    virtual double calculate() {
        double start = getTime();
        
        for (size_t arg0 = 0; arg0<coo->nnz; arg0++) {
            auto item = coo->items[arg0];
            size_t i = item.row;
            size_t j = item.col;
            double val = item.val;
            for (size_t k = 0; k<cols; k++) {
                C[i*cols+k] += val * B[j*cols+k];
            }
        }
        
        double end = getTime();
        return (double)(end-start);
    }
    
protected:
    uint64_t rows, cols;
    COO *coo;
    double *B;
    double *C;
    
    void init(std::string input);
    void initCOO(std::string input);
    void generateDense();
    double getTime();
    void printElapsedTime(double stime, double etime);
private:
    std::vector<std::string> split(std::string line);
};


