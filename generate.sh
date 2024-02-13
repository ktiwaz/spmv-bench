#!/bin/bash

if [ ! -d data ]
then
    mkdir data
fi

cd data

##
## Generate 1024x1024 matricies
##
python3 ../gen.py 1024 1024 8
python3 ../gen.py 1024 1024 16
python3 ../gen.py 1024 1024 32
python3 ../gen.py 1024 1024 512
python3 ../gen.py 1024 1024 1024

##
## Generate 512x512 matricies
##
python3 ../gen.py 512 512 8
python3 ../gen.py 512 512 16
python3 ../gen.py 512 512 32
python3 ../gen.py 512 512 128
python3 ../gen.py 512 512 512

cd ..

