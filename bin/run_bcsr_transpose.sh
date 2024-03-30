#!/bin/bash
bin/bcsr/serial_transpose.sh $1
bin/bcsr/omp_transpose.sh $1
bin/bcsr/omp_gpu_transpose.sh $1
