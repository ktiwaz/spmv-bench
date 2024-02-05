#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <iostream>

class CSR : public SpM {
public:
    CSR() {
        init("../test_rank2.mtx");
        format();
    }
    
    //
    // Our data structures
    //
    uint64_t *rowptr;
    uint64_t *rowidx;
    double *values;
    
    //
    // The format method
    // Called for formats that inherit this base class
    //
    void format() override {
        std::cout << "Formatting CSR" << std::endl;
        
        rowptr = new uint64_t[rows + 1];
        rowptr[rows] = coo->nnz;
        rowidx = new uint64_t[coo->nnz];
        values = new double[coo->nnz];
        
        uint64_t prev_row = -1;
        uint64_t curr_idx = 0;
        for (uint64_t current_nz = 0; current_nz < coo->nnz; current_nz++) {
            auto item = coo->items[current_nz];
            uint64_t current_row = item.row;
            uint64_t current_col = item.col;
            
            for (uint64_t row = prev_row + 1; row <= current_row; row++) {
                rowptr[row] = curr_idx;
            }
            prev_row = current_row;

            rowidx[curr_idx] = item.col;
            values[curr_idx] = item.val;
            curr_idx++;
        }
    }
    
    //
    // The print method for the current sparse matrix
    //
    void printSparse() override {
        std::cout << "Rows: " << coo->rows << " | Cols: " << coo->cols << " | NNZ: " << coo->nnz << std::endl;
        std::cout << "----------" << std::endl;
        std::cout << "Num_Rows: " << rows << std::endl;
        
        std::cout << "rowptr: ";
        for (size_t i = 0; i<rows+1; i++) std::cout << rowptr[i] << ",";
        std::cout << std::endl;
        
        std::cout << "rowidx: ";
        for (size_t i = 0; i<coo->nnz; i++) std::cout << rowidx[i] << ",";
        std::cout << std::endl;
        
        std::cout << "values: ";
        for (size_t i = 0; i<coo->nnz; i++) std::cout << values[i] << ",";
        std::cout << std::endl;
    }
    
    //
    // The calculation algorithm for the current format
    //
    double calculate() override {
        double start = getTime();
        
        for (size_t i = 0; i<rows; i++) {
            for (uint64_t p = rowptr[i]; p<rowptr[i+1]; p++) {
                uint64_t j = rowidx[p];
                for (uint64_t k = 0; k<rows; k++) {
                    C[i*cols+k] += values[p] * B[j*cols+k];
                }
            }
        }
        
        double end = getTime();
        return (double)(end-start);
    }
};

