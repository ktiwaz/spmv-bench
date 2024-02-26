#pragma once

#include <iostream>
#include <string>
#include <cstdlib>

#include <spmm.h>
#include <bell.h>

class Matrix : public BELL {
public:
    explicit Matrix(int argc, char **argv) : BELL(argc, argv) {}
    double calculate() override;
};

