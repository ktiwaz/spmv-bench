#!/bin/bash
bin/coo/serial_transpose.sh $1
bin/coo/omp_transpose.sh $1
