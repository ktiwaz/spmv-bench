#pragma once

#include <iostream>
#include <string>
#include <cstdlib>

#include <spmm.h>
#include <ell.h>

class Matrix : public ELL {
public:
    explicit Matrix(int argc, char **argv) : ELL(argc, argv) {}
    double calculate() override;
};

