#!/bin/bash
bin/bcsr/serial.sh $1
bin/bcsr/omp.sh $1
bin/bcsr/omp_gpu.sh $1
