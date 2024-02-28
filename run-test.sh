#!/bin/bash

bin/run_serial.sh "test"
bin/run_omp.sh "test"
bin/run_omp_collapse.sh "test"
bin/run_transpose_serial.sh "test"
bin/run_transpose_omp.sh "test"
bin/run_transpose_omp_collapse.sh "test"
bin/run_transpose_omp_omp.sh "test"

