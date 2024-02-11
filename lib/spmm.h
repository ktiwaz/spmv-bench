#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include <algorithm>

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
    
    void sort() {
        std::sort(items.begin(), items.end(), [](const COOItem& a, const COOItem& b) {
            return a.row < b.row;
        });
    }
};

//
// The base class for loading and formatting a matrix
//
class SpM {
public:
    SpM(int argc, char **argv);
    void debug();
    void printDense(bool all = true);
    void printResult(bool all = true);
    void benchmark();
    void report();
    
    //
    // The format method
    // Called for formats that inherit this base class
    //
    virtual void format() {
        double start = getTime();
        initCOO();
        double end = getTime();
        formatTime = (double)(end-start);
    }
    
    //
    // The format reporting method
    // This depends on the format
    //
    virtual void reportFormat() {}
    
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
    // Option variables
    std::string input = "";
    int iters = 1;
    int block_rows = 1;
    int block_cols = 1;
    bool printDebug = false;
    
    // Matrix data
    uint64_t rows, cols;
    COO *coo;
    double *B;
    double *C;
    
    // Other data
    double formatTime = 0;
    double benchTime = 0;
    
    // Functions
    void initCOO();
    void generateDense();
    double getTime();
    void printElapsedTime(double stime, double etime);
private:
    std::vector<std::string> split(std::string line);
};


