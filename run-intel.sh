#!/bin/bash

bin/run_coo_transpose.sh "intel"
bin/run_csr_transpose.sh "intel"
bin/run_ell_transpose.sh "intel"
bin/run_bcsr_transpose.sh "intel"

