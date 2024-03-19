#!/bin/bash

echo ""
echo "--------------------"
echo "Running COO"
echo "--------------------"
bin/run_coo2.sh "arm"

echo ""
echo "--------------------"
echo "Running CSR"
echo "--------------------"
bin/run_csr2.sh "arm"

echo ""
echo "--------------------"
echo "Running ELL"
echo "--------------------"
bin/run_ell2.sh "arm"

echo ""
echo "--------------------"
echo "Running BCSR"
echo "--------------------"
bin/run_bcsr2.sh "arm"

echo ""
echo "--------------------"
echo "Running BELL"
echo "--------------------"
bin/run_bell2.sh "arm"

#echo ""
#echo "--------------------"
#echo "Running serial"
#echo "--------------------"
#bin/run_serial.sh "arm"

