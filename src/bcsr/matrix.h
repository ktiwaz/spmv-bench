#pragma once

#include <iostream>
#include <string>
#include <cstdlib>

#include <spmm.h>
#include <bcsr.h>

class Matrix : public BCSR {
public:
    explicit Matrix(int argc, char **argv) : BCSR(argc, argv) {}
    double calculate() override;
};

