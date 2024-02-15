#!/bin/bash

if [ ! -d data ]
then
    mkdir data
fi

data=(
    "bcsstk13"
    "bcsstk17"
    "cant"
    "pdb1HYS"
    "rma10"
    "dw4096"
    "cop20k_A"
    "af23560"
    "x104"
)

cd data

rm -rf ./*

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

wget https://suitesparse-collection-website.herokuapp.com/MM/HB/bcsstk13.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/HB/bcsstk17.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Williams/cant.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Williams/pdb1HYS.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Bova/rma10.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Bai/dw4096.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Williams/cop20k_A.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Bai/af23560.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/DNVS/x104.tar.gz

for d in "${data[@]}"
do
    echo "Unzipping $d"
    tar -xvzf $d.tar.gz
    mv $d/$d.mtx .
    rm -rf ./$d
done

rm *.tar.gz

echo "Done!"

