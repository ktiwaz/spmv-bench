#!/bin/bash
bin/csr/serial_transpose.sh $1
bin/csr/omp_transpose.sh $1
