#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include <map>
#include <cstdio>

class ELL : public SpM {
public:
    explicit ELL(int argc, char **argv) : SpM(argc, argv) {}
    
    //
    // Our data structures
    //
    uint64_t *colidx;
    double *values;
    
    //
    // The format method
    // Called for formats that inherit this base class
    //
    void format() override {
        double start = getTime();
        
        initCOO();
        num_cols = 0;
        
        // Calculate the maximum number of columns per row
        uint64_t max = 0;
        uint64_t buffer = 0;
        //uint64_t current = coo->items[0].row;
        //uint64_t current = coo->nnz;
        uint64_t current = 0;
        for (uint64_t i = 0; i < coo->nnz; i++)
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
        
        // Create a map to make padding more efficient
        current = coo->items[0].row;
        std::map<uint64_t, std::vector<uint64_t>> row_map;
        std::map<uint64_t, std::vector<double>> val_map;
        row_map[current] = std::vector<uint64_t>();
        val_map[current] = std::vector<double>();
        
        for (uint64_t i = 0; i < coo->nnz; i++) {
          uint64_t cur_row = coo->items[i].row;
          if (cur_row != current)
          {
            current = cur_row;
            row_map[current] = std::vector<uint64_t>();
            val_map[current] = std::vector<double>();
          }
          row_map[cur_row].push_back(coo->items[i].col);
          val_map[cur_row].push_back(coo->items[i].val);
        }
        
        /*for (auto it = row_map.begin(); it != row_map.end(); it++)
        {
            std::cout << it->first    // string (key)
                      << ": [";
            for (auto v : it->second) std::cout << v << " ";
            std::cout << "]" << std::endl;
        }*/

        /// Create the column coordinate list
        colidx = new uint64_t[rows * num_cols];
        values = new double[rows * num_cols];
        uint64_t index = 0;
        uint64_t avg_sum = 0;
        
        // Iterate over each row, and add the non-zero elements
        for (uint64_t i = 0; i<rows; i++) {
            uint64_t cols = 0;
            auto current_cols = row_map[i];
            auto current_vals = val_map[i];
            avg_sum += current_cols.size();
            for (uint64_t j = 0; j<current_cols.size(); j++) {
              colidx[index] = current_cols[j];
              values[index] = current_vals[j];
              ++index;
              ++cols;
            }
            
            if (cols < num_cols) {
                // Loop while cols < num_cols
                // - set col = 0
                // - check if col is non-zero
                // -- advance col
                // -- repeat
                int col = 0;
                while (cols < num_cols) {
                    while (col < num_cols) {
                        if (std::find(current_cols.begin(), current_cols.end(), col) == current_cols.end()) {
                          //Not Found
                          break;
                        }
                        ++col;
                    }
                    
                    colidx[index] = col;
                    values[index] = 0;
                    ++index;
                    ++cols;
                    ++col;
                }

            }
        }
        
        double end = getTime();
        formatTime = (double)(end-start);
        
        // Average
        avg_num_cols = avg_sum / rows;
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
        for (uint64_t i = 0; i<(rows*num_cols); i++) {
            std::cout << colidx[i] << ",";
            if (!all && i > 30) break;
        }
        std::cout << std::endl;
        
        std::cout << "values: ";
        for (uint64_t i = 0; i<(rows*num_cols); i++) {
            std::cout << values[i] << ",";
            if (!all && i > 30) break;
        }
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

