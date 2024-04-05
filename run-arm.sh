#!/bin/bash

#bin/run_coo.sh "arm"
#bin/run_csr.sh "arm"
#bin/run_ell.sh "arm"
#bin/run_bcsr.sh "arm"

#bin/run-cusparse.sh "arm"

#bin/run_coo_transpose.sh "arm"
#bin/run_csr_transpose.sh "arm"
#bin/run_ell_transpose.sh "arm"
#bin/run_bcsr_transpose.sh "arm"

bin/coo/omp.sh "arm"
bin/csr/omp.sh "arm"
bin/ell/omp.sh "arm"
bin/bcsr/omp.sh "arm"

bin/coo/omp_threads.sh "arm"
bin/csr/omp_threads.sh "arm"
bin/ell/omp_threads.sh "arm"
bin/bcsr/omp_threads.sh "arm"

bin/coo/omp_transpose.sh "arm"
bin/csr/omp_transpose.sh "arm"
bin/ell/omp_transpose.sh "arm"
bin/bcsr/omp_transpose.sh "arm"

