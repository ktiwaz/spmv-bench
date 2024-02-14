#pragma once

#include <iostream>
#include <string>
#include <cstdlib>

#include <spmm.h>

class Matrix : public SpM {
public:
    explicit Matrix(int argc, char **argv) : SpM(argc, argv) {}
    double calculate() override;
};

