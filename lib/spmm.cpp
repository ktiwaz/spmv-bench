#include <fstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "spmm.h"

//
// The constructor
//
SpM::SpM() {
    init("../test_rank2.mtx");
}

void SpM::init(std::string input) {
    // Test matrix
    // TODO: Change
    initCOO(input);
    rows = coo->rows;
    cols = coo->cols;
    generateDense();
}

//
// Timing functions
//
double SpM::getTime() {
    struct timeval tp;
    int stat = gettimeofday(&tp, NULL);
    return (tp.tv_sec + tp.tv_usec * 1.0e-6);
}

void SpM::printElapsedTime(double stime, double etime) {
    fprintf(stdout, "%lf\n", etime-stime);
}

//
// Calls and collections benchmarking info
//
void SpM::benchmark(size_t iters) {
    std::vector<double> times;
    for (size_t i = 0; i<iters; i++) {
        double t = calculate();
        times.push_back(t);
    }
    
    double avg = 0;
    for (auto t : times) avg += t;
    avg = avg / iters;
    fprintf(stdout, "%lf\n", avg);
}

//
// Prints our dense matrix
//
void SpM::printDense() {
    std::cout << "B: ";
    for (size_t i = 0; i<(rows*cols); i++) {
        std::cout << B[i] << ",";
    }
    std::cout << std::endl;
}

//
// Prints the result matrix
//
void SpM::printResult() {
    std::cout << "C: ";
    for (size_t i = 0; i<(rows*cols); i++) {
        std::cout << C[i] << ",";
    }
    std::cout << std::endl;
}

//
// Generates a dense matrix from a sparse format
// This also generates the result matrix and fills it with zeros
//
void SpM::generateDense() {
    size_t len = rows * cols;
    B = new double[len];
    C = new double[len];
    for (size_t i = 0; i<len; i++) {
        B[i] = 1.7;
        C[i] = 0.0;
    }
}

//
// Reads the file and loads the COO matrix
//
void SpM::initCOO(std::string input) {
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
std::vector<std::string> SpM::split(std::string line) {
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

