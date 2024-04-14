#!/bin/bash

bin/run-cusparse.sh "intel"
bin/coo/gpu_full.sh "intel"
bin/csr/gpu_full.sh "intel"

