#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include <map>
#include <algorithm>

class BCSR : public SpM {
public:
    BCSR(std::string input, uint64_t block_rows, uint64_t block_cols) : SpM(input) {
        this->block_rows = block_rows;
        this->block_cols = block_cols;
        format();
    }
    
    //
    // Our data structures
    //
    uint64_t block_rows = 0;
    uint64_t block_cols = 0;
    
    uint64_t num_blocks;

    uint64_t *colptr;
    uint64_t *colidx;
    uint64_t colptr_len;
    uint64_t colidx_len;

    double *values;
    uint64_t value_len;
    
    //
    // The format method
    // Called for formats that inherit this base class
    //
    void format() override {
        std::cout << "Formatting BCSR" << std::endl;
        
        uint64_t num_nonzeros = coo->nnz;
        
        ///////////////////////////////////////////////
        std::vector<int> A2pos_nc;
        std::vector<int> A2crd;
        std::vector<double> Aval_nc;
        
        ///////////////////////////////////////////////
        // Create a map to make padding more efficient
        uint64_t current = coo->items[0].row;
        std::map<uint64_t, std::vector<uint64_t>> row_map;
        std::map<uint64_t, std::vector<double>> val_map;
        row_map[current] = std::vector<uint64_t>();
        val_map[current] = std::vector<double>();
        
        for (uint64_t i = 0; i < num_nonzeros; i++) {
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
        
        ///////////////////////////////////////////////
        
        // Step 2: Examine the blocks
        // We only want the blocks with values
        //
        // From here, we can start building the A2 dimension
        //
        for (uint64_t i=0; i<rows; i+=block_rows) {
          for (uint64_t j=0; j<cols; j+=block_cols) {
            // Note: for A2_crd, corresponds to j, divide by "block_cols"
            // to get the block position
            
            //printf("[Search] i, j: %d, %d\n", i, j);
            bool has_values = false;
            for (uint64_t bi = i; bi<(i+block_rows); bi++) {
              for (uint64_t bj = j; bj<(j+block_cols); bj++) {
                //printf("--bi, bj: %d, %d\n", bi, bj);
                
                auto current_cols = row_map[bi];
                if (!current_cols.empty()) {
                  if (std::find(current_cols.begin(), current_cols.end(), bj) != current_cols.end()) {
                    //printf("-- -- Found\n");
                    has_values = true;
                    goto s_done;
                  }
                }
                
              }
            }
            s_done:
            
            // Check the block and see if it has values
            // If so, we use it
            //if (has_values(i, i+block_rows, j, j+block_cols, coo_matrix)) {
            if (has_values) {
              A2pos_nc.push_back(i/block_rows);
              A2crd.push_back(j/block_cols);
              
              // Add all the values including the padded zeros
              //
              // To do this, we first search the COO array for a non-zero
              // value. If there is no such value, then we add the
              // index with a zero.
              //
              for (uint64_t bi = i; bi<(i+block_rows); bi++) {
                for (uint64_t bj = j; bj<(j+block_cols); bj++) {
                  bool found = false;
                  //printf("[Val] Bi: %d | Bj: %d\n", bi, bj);
                  
                  auto current_cols = row_map[bi];
                  if (!current_cols.empty()) {
                    auto pos = std::find(current_cols.begin(), current_cols.end(), bj);
                    if (pos != current_cols.end()) {
                      uint64_t pos2 = pos - current_cols.begin();
                      double val = val_map[bi][pos2];
                      //printf("--Found\n");
                      
                      Aval_nc.push_back(val);
                      found = true;
                    }
                  }
                  
                  if (found == false) {
                    Aval_nc.push_back(0);
                  }
                }
              }
            } // end if
            
          }
        }
        
        //printf("------------\n");
        
        // Compress the row coordinates
        std::vector<int> A2pos;
        A2pos.push_back(0);
        
        int curr = A2pos_nc[0];
        int curr_end = 1;
        for (uint64_t i = 1; i<A2pos_nc.size(); i++) {
          if (A2pos_nc[i] != curr) {
            A2pos.push_back(curr_end);
            curr = A2pos_nc[i];
          }
          curr_end += 1;
        }
        A2pos.push_back(curr_end);
        
        int A1pos = A2pos.size() - 1;
        
        // Copy all the elements over
        num_blocks = A1pos;
        colptr_len = A2pos.size();
        colidx_len = A2crd.size();
        value_len = Aval_nc.size();
        
        colptr = new uint64_t[A2pos.size()];
        colidx = new uint64_t[A2crd.size()];
        values = new double[Aval_nc.size()];
        
        for (uint64_t i = 0; i<A2pos.size(); i++) colptr[i] = A2pos[i];
        for (uint64_t i = 0; i<A2crd.size(); i++) colidx[i] = A2crd[i];
        for (uint64_t i = 0; i<Aval_nc.size(); i++) values[i] = Aval_nc[i];
    }
    
    //
    // The print method for the current sparse matrix
    //
    void printSparse() override {
        std::cout << "Rows: " << coo->rows << " | Cols: " << coo->cols << " | NNZ: " << coo->nnz << std::endl;
        std::cout << "----------" << std::endl;
        std::cout << "num_blocks: " << num_blocks << std::endl;
        std::cout << "block_rows: " << block_rows << std::endl;
        std::cout << "block_cols: " << block_cols << std::endl;
        
        std::cout << "colptr: ";
        for (uint64_t i = 0; i<colptr_len; i++) std::cout << colptr[i] << ",";
        std::cout << std::endl;
        
        std::cout << "colidx: ";
        for (uint64_t i = 0; i<colidx_len; i++) std::cout << colidx[i] << ",";
        std::cout << std::endl;
        
        std::cout << "values: ";
        for (uint64_t i = 0; i<value_len; i++) std::cout << values[i] << ",";
        std::cout << std::endl;
    }
    
    //
    // The calculation algorithm for the current format
    //
    double calculate() override {
        double start = getTime();
        
        for (uint64_t n1 = 0; n1<num_blocks; n1++) {
            for (uint64_t bi = 0; bi<block_rows; bi++) {
                for (uint64_t n2 = colptr[n1]; n2<colptr[n1+1]; n2++) {
                    for (uint64_t bj = 0; bj<block_cols; bj++) {
                        for (uint64_t k = 0; k<rows; k++) {
                            uint64_t i = n1 * block_rows + bi;
                            uint64_t j = colidx[n2] * block_cols + bj;
                            uint64_t index = n2*(block_rows*block_cols) + bi * block_cols + bj;
                            
                            C[i*cols+k] += values[index] * B[i*cols+j];
                        }
                    }
                }
            }
        }
        
        double end = getTime();
        return (double)(end-start);
    }
};

