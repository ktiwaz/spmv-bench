#!/bin/bash

echo ""
echo "--------------------"
echo "Running COO"
echo "--------------------"
bin/run_coo.sh "carina_O3"

echo ""
echo "--------------------"
echo "Running CSR"
echo "--------------------"
bin/run_csr.sh "carina_O3"

echo ""
echo "--------------------"
echo "Running ELL"
echo "--------------------"
bin/run_ell.sh "carina_O3"

echo ""
echo "--------------------"
echo "Running BCSR"
echo "--------------------"
bin/run_bcsr.sh "carina_O3"

echo ""
echo "--------------------"
echo "Running BELL"
echo "--------------------"
bin/run_bell.sh "carina_O3"

echo ""
echo "--------------------"
echo "Running serial"
echo "--------------------"
bin/run_serial.sh "carina_O3"

