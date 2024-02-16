#pragma once

#include <iostream>
#include <string>
#include <cstdlib>

#include <spmm.h>
#include <csr.h>

class Matrix : public CSR {
public:
    explicit Matrix(int argc, char **argv) : CSR(argc, argv) {}
    double calculate() override;
};

