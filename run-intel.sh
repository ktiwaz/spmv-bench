#!/bin/bash

bin/coo/serial.sh "intel"
bin/csr/serial.sh "intel"
bin/ell/serial.sh "intel"
bin/bcsr/serial.sh "intel"

bin/coo/omp.sh "intel"
bin/csr/omp.sh "intel"
bin/ell/omp.sh "intel"
bin/bcsr/omp.sh "intel"

mv data data.old
mv data_gpu data

bin/coo/omp_gpu.sh "intel"
bin/csr/omp_gpu.sh "intel"
bin/ell/omp_gpu.sh "intel"
bin/bcsr/omp_gpu.sh "intel"

mv data data_gpu
mv data.old data

