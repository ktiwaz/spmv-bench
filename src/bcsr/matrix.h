#pragma once

#include <iostream>
#include <string>
#include <cstdlib>

#include <spmm.h>
#include <bcsr.h>

class Matrix : public BCSR {
public:
    explicit Matrix(int argc, char **argv) : BCSR(argc, argv) {}
    
    //
    // This loads a formatted BCSR matrix
    // Format:
    // Line 1): Rows
    // Line 2): Cols
    // Line 3): NNZ
    // Line 4): colptr_len
    // Line <colptr_len>: colptr
    // Line: colidx_len
    // Line <colidx_len>: colidx
    // Line: value_len
    //
    /*void format() override {
        coo = new COO;
        std::ifstream reader(input);
        if (!reader.is_open()) {
            std::cerr << "Error: Unable to open matrix." << std::endl;
            std::cerr << "--> " << input << std::endl;
            return;
        }
        
        std::string line = "";
        
        // Rows, cols, nnz
        std::getline(reader, line);
        coo->rows = std::stoull(line);
        
        std::getline(reader, line);
        coo->cols = std::stoull(line);
        
        std::getline(reader, line);
        coo->nnz = std::stoull(line);
        
        // colptr_len
        std::getline(reader, line);
        colptr_len = std::stoull(line);
        colptr = new double[colptr_len];
        
        for (uint64_t i = 0; i<colptr_len; i++) {
            std::getline(reader, line);
        }
    }*/
    
    double calculate() override;
};

