#include <fstream>
#include <iostream>

#include "spmm.h"

//
// The constructor
//
Matrix::Matrix() {
    // Test matrix
    // TODO: Change
    std::string input = "../square.mtx";
    initCOO(input);
}

//
// Generates a dense matrix from a sparse format
//
double *Matrix::generateDense() {
    size_t denseLen = coo->rows * coo->cols;
    double *mtx = new double[denseLen];
    for (int i = 0; i<denseLen; i++) {
        mtx[i] = 0;
    }
    for (auto item : coo->items) {
        mtx[item.row * coo->cols + item.col] = item.val;
    }
    return mtx;
}

//
// Reads the file and loads the COO matrix
//
void Matrix::initCOO(std::string input) {
    coo = new COO;
    std::ifstream reader(input);
    if (!reader.is_open()) {
        std::cerr << "Error: Unable to open matrix." << std::endl;
        std::cerr << "--> " << input << std::endl;
        return;
    }
    
    std::string line = "";
    bool foundHeader = false;
    while (std::getline(reader, line)) {
        if (line.length() == 0) continue;
        if (line[0] == '%') continue;
        
        // If we haven't found a header, we need to process it
        if (foundHeader == false) {
            auto words = split(line);
            coo->rows = std::stoi(words[0]);
            coo->cols = std::stoi(words[1]);
            coo->nnz = std::stoi(words[2]);
        
            foundHeader = true;
            continue;
        }
        
        auto words = split(line);
        COOItem item;
        item.row = std::stoi(words[0]) - 1;
        item.col = std::stoi(words[1]) - 1;
        item.val = std::stod(words[2]);
        coo->items.push_back(item);
    }
}

//
// Splits a string into words
//
std::vector<std::string> Matrix::split(std::string line) {
    std::vector<std::string> words;
    std::string buffer = "";
    for (char c : line) {
        if (c == ' ') {
            if (buffer.length() > 0) {
                words.push_back(buffer);
                buffer = "";
            }
        } else {
            buffer += c;
        }
    }
    if (buffer.length() > 0) words.push_back(buffer);
    return words;
}

