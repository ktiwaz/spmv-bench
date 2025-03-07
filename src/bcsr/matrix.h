#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

#include <spmm.h>
#include <bcsr.h>

class Matrix : public BCSR {
public:
    explicit Matrix(int argc, char **argv) : BCSR(argc, argv) {}
    
    //
    // This loads a formatted BCSR matrix
    // Format:
    // Line: colptr_len
    // Line <colptr_len>: colptr
    // Line: colidx_len
    // Line <colidx_len>: colidx
    // Line: value_len
    //
    // void format() override {
    //     double start = getTime();
    //     initCOO();
        
    //     std::ifstream reader(output);
    //     if (!reader.is_open()) {
    //         std::cerr << "Error: Unable to open BCSR matrix." << std::endl;
    //         std::cerr << "--> " << input << std::endl;
    //         return;
    //     }
        
    //     std::string line = "";
        
    //     // num_blocks
    //     std::getline(reader, line);
    //     num_blocks = std::stoull(line);
        
    //     // colptr_len
    //     std::getline(reader, line);
    //     colptr_len = std::stoull(line);
    //     colptr = new uint64_t[colptr_len];
        
    //     for (uint64_t i = 0; i<colptr_len; i++) {
    //         std::getline(reader, line);
    //         colptr[i] = std::stoull(line);
    //     }
        
    //     // colidx_len
    //     std::getline(reader, line);
    //     colidx_len = std::stoull(line);
    //     colidx = new uint64_t[colidx_len];
        
    //     for (uint64_t i = 0; i<colidx_len; i++) {
    //         std::getline(reader, line);
    //         colidx[i] = std::stoull(line);
    //     }
        
    //     // value_len
    //     std::getline(reader, line);
    //     value_len = std::stoull(line);
    //     values = new double[value_len];
        
    //     for (uint64_t i = 0; i<value_len; i++) {
    //         std::getline(reader, line);
    //         values[i] = std::stod(line);
    //     }
        
    //     double end = getTime();
    //     formatTime = (double)(end-start);
    // }
    
    double calculate() override;
};

