#!/bin/bash

bin/coo/omp.sh "intel_k"
bin/csr/omp.sh "intel_k"
bin/ell/omp.sh "intel_k"
bin/bcsr/omp.sh "intel_k"

#bin/run_coo_transpose.sh "intel"
#bin/run_csr_transpose.sh "intel"
#bin/run_ell_transpose.sh "intel"
#bin/run_bcsr_transpose.sh "intel"

