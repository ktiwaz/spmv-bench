#!/bin/bash
bin/ell/serial_transpose.sh $1
bin/ell/omp_transpose.sh $1
bin/ell/omp_gpu_transpose.sh $1
