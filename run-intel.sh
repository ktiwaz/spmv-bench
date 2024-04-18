#!/bin/bash

bin/coo/run_coo_transpose.sh "intel"
bin/csr/run_csr_transpose.sh "intel"
bin/ell/run_ell_transpose.sh "intel"
bin/bcsr/run_bcsr_transpose.sh "intel"

