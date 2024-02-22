#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include <map>
#include <cstdio>

class BELL : public SpM {
public:
    explicit BELL(int argc, char **argv) : SpM(argc, argv) {}
    
    //
    // Our data structures
    //
    uint64_t blocks = 0;
    uint64_t *rowptr;
    uint64_t *colidx;
    double *values;
    uint64_t num_cols = 0;
    
    //
    // The format method
    // Called for formats that inherit this base class
    //
    void format() override {
        double start = getTime();
        
        initCOO();
        num_cols = 0;
        blocks = block_rows;
        //std::cout << "BLOCKS: " << blocks << std::endl;
        
        std::vector<int> block_num_cols;
        
        // Create a map to make padding more efficient
        uint64_t current = coo->items[0].row;
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
        
        
        // debug
        /*for (auto it = row_map.begin(); it != row_map.end(); it++)
        {
            std::cout << it->first    // string (key)
                      << ": [";
            for (auto v : it->second) std::cout << v << " ";
            std::cout << "]" << std::endl;
        }*/
        // end debug
        
        std::map<uint64_t, uint64_t> row_blocks;
        //uint64_t index = 0;
        
        // Calculate the maximum number of columns per row per block
        for (uint64_t bi = 0; bi<rows; bi += blocks) {
            uint64_t max = 0;
            
            for (uint64_t i = bi; i<(bi+blocks); i++) {
                auto current_cols_size = row_map[i].size();
                if (current_cols_size > max) max = current_cols_size;
                //std::cout << i << std::endl;
            }
            row_blocks[bi] = max;
            
            for (uint64_t i = bi; i<(bi+blocks); i++) row_blocks[i] = max;
            
            //++index;
            
            //std::cout << "--> Max: " << max << std::endl;
            //std::cout << "---" << std::endl;
        }
        
        // debug
        /*for (auto it = row_blocks.begin(); it != row_blocks.end(); it++)
        {
            std::cout << it->first    // string (key)
                      << ": [";
            std::cout << it->second;
            std::cout << "]" << std::endl;
        }*/
        // end debug
        
        rowptr = new uint64_t[rows+1];
        //for (uint64_t i = 0; i<rows+1; i++) rowptr[i] = 0;
        rowptr[0] = 0;
        for (uint64_t i = 1, bi = 0; i<rows+1; i++, bi++) {
            //std::cout << "i: " << i << " | bi: " << bi << " -> ";
            //    std::cout << "rowptr[i]: " << rowptr[i] << " | rowptr[i-1]: " << rowptr[i-1] << " | row_blocks[bi]: " << row_blocks[bi] << std::endl;
            rowptr[i] = rowptr[i-1] + row_blocks[bi];
        }
        
        // Now, start building the final b-ell structures
        std::vector<uint64_t> colidx1;
        std::vector<double> values1;
        uint64_t avg_sum = 0;
        
        for (uint64_t bi = 0; bi<rows; bi += blocks) {
            uint64_t max_cols = row_blocks[bi];
            //std::cout << "Max_cols: " << max_cols << std::endl;
            
            for (uint64_t i = bi; i<(bi+blocks); i++) {
                uint64_t cols = 0;
                auto current_cols = row_map[i];
                auto current_vals = val_map[i];
                avg_sum += current_cols.size();
                for (uint64_t j = 0; j<current_cols.size(); j++) {
                  colidx1.push_back(current_cols[j]);
                  values1.push_back(current_vals[j]);
                  //++index;
                  ++cols;
                }
                
                if (cols < max_cols) {
                    // Loop while cols < num_cols
                    // - set col = 0
                    // - check if col is non-zero
                    // -- advance col
                    // -- repeat
                    int col = 0;
                    while (cols < max_cols) {
                        while (col < max_cols) {
                            if (std::find(current_cols.begin(), current_cols.end(), col) == current_cols.end()) {
                              //Not Found
                              break;
                            }
                            ++col;
                        }
                        
                        colidx1.push_back(col);
                        values1.push_back(0);
                        //++index;
                        ++cols;
                        ++col;
                    }

                }
            }
        }
        
        num_cols = colidx1.size();
        colidx = new uint64_t[num_cols];
        values = new double[num_cols];
        for (uint64_t i = 0; i<num_cols; i++) {
            colidx[i] = colidx1[i];
            values[i] = values1[i];
        }
        
        double end = getTime();
        formatTime = (double)(end-start);
        
        // Average
        //avg_num_cols = avg_sum / rows;
    }
    
    //
    // The print method for the current sparse matrix
    //
    void printSparse(bool all = true) override {
        std::cout << "Rows: " << coo->rows << " | Cols: " << coo->cols << " | NNZ: " << coo->nnz << std::endl;
        std::cout << "----------" << std::endl;
        std::cout << "row: " << rows << std::endl;
        
        std::cout << "rowptr: ";
        for (uint64_t i = 0; i<rows+1; i++) {
            std::cout << rowptr[i] << ",";
            if (!all && i > 30) break;
        }
        std::cout << std::endl;
        
        std::cout << "colidx: ";
        for (uint64_t i = 0; i<num_cols; i++) {
            std::cout << colidx[i] << ",";
            if (!all && i > 30) break;
        }
        std::cout << std::endl;
        
        std::cout << "values: ";
        for (uint64_t i = 0; i<num_cols; i++) {
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
            for (uint64_t p = rowptr[i]; p<rowptr[i+1]; p++) {
                uint64_t j = colidx[p];
                uint64_t val = values[p];
                for (uint64_t k = 0; k<rows; k++) {
                    C[i*cols+j] += val * B[k*cols+j];
                }
            }
        }
        
        double end = getTime();
        return (double)(end-start);
    }
};

