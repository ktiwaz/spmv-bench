#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

#include <spmm.h>
#include <bcsr.h>

class BCSRMatrix : public BCSR {
public:
    explicit BCSRMatrix(int argc, char **argv) : BCSR(argc, argv) {}
    
    // We dump in this format:
    // <rows> <cols> <nnz>
    // <colptr_len> <colidx_len> <value_len>
    // all items here....
    void dump() {
        std::cout << "Saving to: " << output << std::endl;
        
        std::ofstream writer(output);
        writer << coo->rows << std::endl;
        writer << coo->cols << std::endl;
        writer << coo->nnz << std::endl;
        
        writer << colptr_len << std::endl;
        for (uint64_t i = 0; i<colptr_len; i++) writer << colptr[i] << std::endl;
        
        writer << colidx_len << std::endl;
        for (uint64_t i = 0; i<colidx_len; i++) writer << colptr[i] << std::endl;
        
        writer << value_len << std::endl;
        for (uint64_t i = 0; i<value_len; i++) writer << values[i] << std::endl;
        
        writer.close();
    }
};

int main(int argc, char **argv) {
    BCSRMatrix mtx(argc, argv);
    mtx.format();
    mtx.dump();
    
    return 0;
}

