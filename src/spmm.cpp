#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <cmath>
#include <omp.h>

#include "spmm.h"

//
// The constructor
//
SpM::SpM(int argc, char **argv) {
    for (int i = 1; i<argc; i++) {
        std::string arg = argv[i];
        if (arg == "--iters") {
            arg = argv[i+1];
            iters = std::stoi(arg);

            i += 1;
        } else if (arg == "--block") {
            arg = argv[i+1];
            i += 1;
            int b = std::stoi(arg);
            block_rows = b;
            block_cols = b;
        } else if (arg == "--threads") {
            arg = argv[i+1];
            threads = std::stoi(arg);
            i += 1;
        } else if (arg == "--thread-list") {
            arg = argv[i+1];
            i += 1;
            std::vector<std::string> thread_str = split(arg, ',');
            for (auto ts : thread_str)
                thread_list.push_back(std::stoi(ts));
            benchmark_threads = true;
        } else if (arg == "--output") {
            output = argv[i+1];
            i += 1;
        } else if (arg == "--debug") {
            printDebug = true;
        } else if (arg[0] == '-') {
            std::cerr << "Error: Invalid option: " << arg << std::endl;
        } else {
            input = arg;
        }
    }
}

void SpM::debug() {
    std::cout << "----------------------------" << std::endl;
    std::cout << "<COMMAND LINE ARGS>" << std::endl;
    std::cout << std::endl;
    std::cout << "Input file: " << input << std::endl;
    std::cout << "Iters: " << iters << std::endl;
    std::cout << "Blocks: <Rows: " << block_rows << ", Cols: " << block_cols << ">" << std::endl;
    std::cout << "Threads: " << threads << std::endl;
    std::cout << "----------------------------" << std::endl;
    
    printSparse(false);
    std::cout << "-----------------" << std::endl;
    printDense(false);
    std::cout << "-----------------" << std::endl;
    printResult(false);
    std::cout << "-----------------" << std::endl;
    
    std::cout << "Verification: " << verifyResults << std::endl;
    std::cout << "MFLOPS: " << benchFlops / 1.0e6 << std::endl;
}

//
// Timing functions
//
double SpM::getTime() {
    struct timeval tp;
    int stat = gettimeofday(&tp, NULL);
    return (tp.tv_sec + tp.tv_usec * 1.0e-6);
}

double SpM::getTime2() {
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
void SpM::_benchmark(double *t, double *f) {
    // If we have threading, we need to set the thread count
    if (threads != -1) omp_set_num_threads(threads);
    std::vector<double> times;
    std::vector<double> flops;

    for (size_t i = 0; i<iters; i++) {
        if (i>0) for (size_t i = 0; i<(cols); i++) C[i] = 0.0;
        double t = calculate();
        times.push_back(t);
        
        double f = getFlopCount();
        double fs = f / t;
        flops.push_back(fs);
    }
    
    double avg = 0;
    for (auto t : times) avg += t;
    *t = avg / iters;
    
    double f_avg = 0;
    for (auto f : flops) f_avg += f;
    *f = f_avg / iters;
}

void SpM::benchmark() {
    // Thread-by-thread evaluation to find the best thread count
    if (benchmark_threads) {
        double bestTime = 0;
        double bestFlops = 0;
        int bestThreads = 0;
        
        if (printDebug) std::cout << "Running Thread Benchmark..." << std::endl;
        
        for (int t : thread_list) {
            threads = t;
            double time = 0;
            double flops = 0;
            _benchmark(&time, &flops);
            
            if (printDebug)
                std::cout << "Thread: " << t << " | Time: " << time << " | FLOPS: " << flops << std::endl;
            
            if (flops > bestFlops) {
                bestTime = time;
                bestFlops = flops;
                bestThreads = t;
            }
        }
        
        benchTime = bestTime;
        benchFlops = bestFlops;
        threads = bestThreads;
        
        if (printDebug) std::cout << "Best thread: " << threads << std::endl;
    
    // Normal benchmarking
    } else {
        double time = 0;
        double flops = 0;
        _benchmark(&time, &flops);
        benchTime = time;
        benchFlops = flops;
    }
}

//
// Computes matrix statistics
//
void SpM::computeStats() {
    // Build the map
    uint64_t current = coo->items[0].row;
    std::map<uint64_t, std::vector<uint64_t>> row_map;
    row_map[current] = std::vector<uint64_t>();
    
    for (uint64_t i = 0; i < coo->nnz; i++) {
      uint64_t cur_row = coo->items[i].row;
      if (cur_row != current)
      {
        current = cur_row;
        row_map[current] = std::vector<uint64_t>();
      }
      row_map[cur_row].push_back(coo->items[i].col);
    }
    
    //
    // Start computing the stats
    //
    // First, get the longest row
    uint64_t max = 0;
    uint64_t max_row = 0;
    for (uint64_t i = 0; i<rows; i++) {
        uint64_t sz = row_map[i].size();
        if (sz > max) {
            max = sz;
            max_row = i;
        }
    }
    
    max_num_cols = max;
    
    // Second, compute the average number of columns per row
    uint64_t size = 0;
    for (uint64_t i = 0; i<rows; i++) {
        size += row_map[i].size();
    }
    
    avg_num_cols = size / rows;
    
    // Third, compute the variance of the number of columns per row
    size = 0;
    for (uint64_t i = 0; i<rows; i++) {
        uint64_t sq = row_map[i].size() - avg_num_cols;
        size += (sq * sq);
    }
    
    variance = size / rows;
    
    // Fourth, compute the standard deviation
    std_deviation = sqrt(variance);
    
    // Print the map if desired
    if (printDebug) {
        /*for (auto it = row_map.begin(); it != row_map.end(); it++)
        {
            std::cout << it->first    // string (key)
                      << ": [";
            for (auto v : it->second) std::cout << v << " ";
            std::cout << "]" << std::endl;
        }
        
        std::cout << "----------------------" << std::endl;*/
    }
}

//
// The reporting method
//
void SpM::report() {
    computeStats();
    
    // Print debug information
    // In reality, this would never be printed in CSV
    if (printDebug) {
        debug();
        std::cout << "----------------------" << std::endl;
    }

    // Print timing information
    fprintf(stdout, "%lf", benchFlops);
    fprintf(stdout, ",%lf", benchFlops / 1.0e6);
    fprintf(stdout, ",%lf", benchFlops / 1.0e9);
    fprintf(stdout, ",%ld", getFlopCount());
    fprintf(stdout, ",%lf", benchTime);
    fprintf(stdout, ",%lf", formatTime);
    fprintf(stdout, ",%lf", benchTime + formatTime);
    
    // Print verification information
    fprintf(stdout, ",%ld", verifyResults);
    
    // Print configuration
    fprintf(stdout, ",%d", iters);
    fprintf(stdout, ",%d,%d", block_rows, block_cols);
    fprintf(stdout, ",%d", threads);
    
    // Print matrix stats
    fprintf(stdout, ",%ld,%ld,%ld", coo->rows, coo->cols, coo->nnz);
    fprintf(stdout, ",%ld", coo->nnz * 2);
    fprintf(stdout, ",%ld", max_num_cols);
    fprintf(stdout, ",%ld", avg_num_cols);
    fprintf(stdout, ",%lf", (double)(max_num_cols / avg_num_cols));
    fprintf(stdout, ",%ld", variance);
    fprintf(stdout, ",%ld", std_deviation);
    
    fprintf(stdout, "\n");
}

//
// A verification method
//
void SpM::verify() {
    // Perform COO calculation
    // We assume COO is correct- regular dense would take too long
    for (size_t arg0 = 0; arg0<coo->nnz; arg0++) {
        auto item = coo->items[arg0];
        size_t i = item.row;
        size_t j = item.col;
        double val = item.val;
        C_check[i] += val * B[j];
    }
    
    // Now, perform the verification
    uint64_t results = 0;
    size_t lens = rows;
    for (size_t i = 0; i<lens; i++) {
        //if ((uint64_t)C[i] != (uint64_t)C_check[i]) {
        if (fabs(C_check[i] - C[i]) > 0.01 * fabs(C_check[i])) {
            ++results;
            //if (results<30) std::cout << C[i] << " | " << C_check[i] << std::endl;
            //if (results<30) std::cout << fabs(C_check[i] - C[i]) << " | " << 0.01 * fabs(C_check[i]) << std::endl;
        }
    }
    
    verifyResults = results;
}

//
// Prints our dense matrix
//
void SpM::printDense(bool all) {
    std::cout << "B: ";
    for (size_t i = 0; i < cols; i++) {
        std::cout << B[i] << ",";
        if (i == 20 && !all) break;
    }
    std::cout << std::endl;
}

//
// Prints the result matrix
//
void SpM::printResult(bool all) {
    std::cout << "C: ";
    for (size_t i = 0; i < rows; i++) {
        std::cout << C[i] << ",";
        if (i == 50 && !all) break;
    }
    std::cout << std::endl;
}
//
// Generates a dense matrix from a sparse format
// This also generates the result matrix and fills it with zeros
//
void SpM::generateDense() {
    size_t len_B = cols;
    size_t len_C = rows;
    B = new double[len_B];
    C = new double[len_C];
    C_check = new double[len_C];
    for (size_t i = 0; i<len_B; i++) {
        B[i] = (double)i;
    }

    for (size_t i = 0; i<len_C; i++) {
        C[i] = 0.0;
        C_check[i] = 0.0;
    }

}

//
// Reads the file and loads the COO matrix
//
void SpM::initCOO() {
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
            coo->rows = std::stoull(words[0]);
            coo->cols = std::stoull(words[1]);
            coo->nnz = std::stoull(words[2]);
        
            foundHeader = true;
            continue;
        }
        
        auto words = split(line);
        COOItem item;
        item.row = std::stoull(words[0]) - 1;
        item.col = std::stoull(words[1]) - 1;
        item.val = std::stod(words[2]);
        coo->items.push_back(item);
    }
    
    // Sort it
    coo->sort();
    
    // Set the row and column counts
    rows = coo->rows;
    cols = coo->cols;
    generateDense();
}

//
// Splits a string into words
//
std::vector<std::string> SpM::split(std::string line, char sp) {
    std::vector<std::string> words;
    std::string buffer = "";
    for (char c : line) {
        if (c == sp) {
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

