#!/bin/bash

bin/run_serial.sh "carina"
bin/run_omp.sh "carina"
bin/run_omp_collapse.sh "carina"
bin/run_transpose_serial.sh "carinia"
bin/run_transpose_omp.sh "carina"
bin/run_transpose_omp_collapse.sh "carina"
bin/run_transpose_omp_omp.sh "carina"

