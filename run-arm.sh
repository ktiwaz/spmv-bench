#!/bin/bash

echo ""
echo "--------------------"
echo "Running COO"
echo "--------------------"
bin/run_coo.sh "arm"

echo ""
echo "--------------------"
echo "Running CSR"
echo "--------------------"
bin/run_csr.sh "arm"

echo ""
echo "--------------------"
echo "Running ELL"
echo "--------------------"
bin/run_ell.sh "arm"

echo ""
echo "--------------------"
echo "Running BCSR"
echo "--------------------"
bin/run_bcsr.sh "arm"

echo ""
echo "--------------------"
echo "Running BELL"
echo "--------------------"
bin/run_bell.sh "arm"

echo ""
echo "--------------------"
echo "Running serial"
echo "--------------------"
bin/run_serial.sh "arm"

#bin/run_serial.sh "carina"
#bin/run_omp.sh "carina"
#bin/run_omp_collapse.sh "carina"
#bin/run_transpose_serial.sh "carinia"
#bin/run_transpose_omp.sh "carina"
#bin/run_transpose_omp_collapse.sh "carina"
#bin/run_transpose_omp_omp.sh "carina"

