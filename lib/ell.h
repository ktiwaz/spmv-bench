#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include <map>

class ELL : public SpM {
public:
    ELL(std::string input) : SpM(input) {
        format();
    }
    
    //
    // Our data structures
    //
    uint64_t num_cols;
    uint64_t *colidx;
    double *values;
    
    //
    // The format method
    // Called for formats that inherit this base class
    //
    void format() override {
        num_cols = 0;
        
        // Calculate the maximum number of columns per row
        uint64_t max = 0;
        uint64_t buffer = 0;
        uint64_t current = coo->items[0].row;
        //uint64_t current = coo->nnz;
        for (uint64_t i = 1; i < coo->nnz; i++)
        {
          if (coo->items[i].row == current)
          {
            ++buffer;
          }
          else
          {
            if (buffer > max)
              max = buffer;
            buffer = 1;
            current = coo->items[i].row;
          }
        }
        if (buffer > max)
          max = buffer;
        num_cols = max;
        
        // Create the column index rows
        colidx = new uint64_t[rows * cols]; //num_cols
        values = new double[rows * cols];   //num_cols
        uint64_t index = 0;

        /// Build the column coordinates/value array
        for (uint64_t i = 0; i < rows; i++)
        {
          /// In this loop, get all non-zero column coordinates and track
          /// how many we have found
          uint64_t found_cols = 0;
          for (uint64_t j = 0; j < coo->nnz; j++)
          {
            if (coo->items[j].row == i)
            {
              ++found_cols;
            }
          }

          /// If the number of columns we have found in the row is less than
          /// the block, we need to add some zeros to create the block
          if (found_cols == num_cols)
          {
            for (uint64_t j = 0; j < coo->nnz; j++)
            {
              if (coo->items[j].row == i)
              {
                colidx[index] = coo->items[j].col;
                values[index] = coo->items[j].val;
                ++index;
              }
            }
            continue;
          }

          /// If there were no found columns, we'll just add zeros
          if (found_cols == 0)
          {
            for (uint64_t j = 0; j < num_cols; j++)
            {
              colidx[index] = j;
              values[index] = 0;
              ++index;
            }
            continue;
          }

          /// If we have an odd number, we need to add preceeding elements before
          /// the actual non-zero indicies
          for (uint64_t j = 0; j < coo->nnz; j++)
          {
            if (coo->items[j].row == i)
            {
              // TODO: This IS NOT portable
              for (uint64_t k = 0; k < coo->items[j].col && found_cols < num_cols; k++)
              {
                colidx[index] = k;
                values[index] = 0;
                ++index;
                ++found_cols;
              }
              colidx[index] = coo->items[j].col;
              values[index] = coo->items[j].val;
              ++index;
            }
          }
        }
    }
    
    //
    // The print method for the current sparse matrix
    //
    void printSparse(bool all = true) override {
        std::cout << "Rows: " << coo->rows << " | Cols: " << coo->cols << " | NNZ: " << coo->nnz << std::endl;
        std::cout << "----------" << std::endl;
        std::cout << "row: " << rows << std::endl;
        std::cout << "cols: " << num_cols << std::endl;
        
        std::cout << "colidx: ";
        for (uint64_t i = 0; i<(rows*num_cols); i++) std::cout << colidx[i] << ",";
        std::cout << std::endl;
        
        std::cout << "values: ";
        for (uint64_t i = 0; i<(rows*num_cols); i++) std::cout << values[i] << ",";
        std::cout << std::endl;
    }
    
    //
    // The calculation algorithm for the current format
    //
    double calculate() override {
        double start = getTime();
        
        for (uint64_t i = 0; i<rows; i++) {
            for (uint64_t n1 = 0; n1<num_cols; n1++) {
                uint64_t p = i * num_cols + n1;
                uint64_t j = colidx[p];
                for (uint64_t k = 0; k<rows; k++) {
                    C[i*cols+k] += values[p] * B[j*cols+k];
                }
            }
        }
        
        double end = getTime();
        return (double)(end-start);
    }
};

