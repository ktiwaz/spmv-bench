#!/bin/bash

bin/coo/cusparse.sh $1
bin/coo/gpu_full.sh $1

bin/csr/cusparse.sh $1
bin/csr/gpu_full.sh $1

