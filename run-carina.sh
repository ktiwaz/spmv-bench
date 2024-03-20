#!/bin/bash

#echo ""
#echo "--------------------"
#echo "Running BELL"
#echo "--------------------"
#bin/run_bell.sh "carina"

#echo ""
#echo "--------------------"
#echo "Running serial"
#echo "--------------------"
#bin/run_serial.sh "carina"

## Part 2- transpose
echo ""
echo "--------------------"
echo "Running COO transpose"
echo "--------------------"
bin/run_coo2.sh "carina"

echo ""
echo "--------------------"
echo "Running CSR transpose"
echo "--------------------"
bin/run_csr2.sh "carina"

echo ""
echo "--------------------"
echo "Running ELL transpose"
echo "--------------------"
bin/run_ell2.sh "carina"

echo ""
echo "--------------------"
echo "Running BCSR transpose"
echo "--------------------"
bin/run_bcsr2.sh "carina"

echo ""
echo "--------------------"
echo "Running BELL transpose"
echo "--------------------"
bin/run_bell2.sh "carina"


