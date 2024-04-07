#!/bin/bash

#bin/run_coo.sh "intel"
#bin/run_csr.sh "intel"
#bin/run_ell.sh "intel"
#bin/run_bcsr.sh "intel"

#bin/run_coo_transpose.sh "intel"
#bin/run_csr_transpose.sh "intel"
#bin/run_ell_transpose.sh "intel"
bin/run_bcsr_transpose.sh "intel"

#bin/coo/omp.sh "intel"
#bin/csr/omp.sh "intel"
#bin/ell/omp.sh "intel"
#bin/bcsr/omp.sh "intel"

bin/coo/omp_threads.sh "intel"
bin/csr/omp_threads.sh "intel"
bin/ell/omp_threads.sh "intel"
bin/bcsr/omp_threads.sh "intel"

bin/coo/omp_transpose.sh "intel"
bin/csr/omp_transpose.sh "intel"
bin/ell/omp_transpose.sh "intel"
bin/bcsr/omp_transpose.sh "intel"

