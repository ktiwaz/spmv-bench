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
    
    "shallow_water1" #81,920
    "2cubes_sphere"
    "scircuit"
    "mac_econ_fwd500"
    "webbase-1M"
    "hood"      # 220,542
    "bmw3_2"    # 227,362
    "pre2"      # 659,033
    
    "pwtk"          # 217,918
    "crankseg_2"    # 63,838
    "torso1"        # 116,158
    "atmosmodd"     # 1,270,432
    "msdoor"        # 415,863
    "F1"            # 343,791
    "nd24k"         # 72,000
    "inline_1"      # 503,712
    "ldoor"         # 952,203
    "cage14"        # 1,505,785
)

##
## Generate 1024x1024 matricies
##
#python3 ../gen.py 1024 1024 8
#python3 ../gen.py 1024 1024 16
#python3 ../gen.py 1024 1024 32
#python3 ../gen.py 1024 1024 512
#python3 ../gen.py 1024 1024 1024

##
## Generate 512x512 matricies
##
#python3 ../gen.py 512 512 8
#python3 ../gen.py 512 512 16
#python3 ../gen.py 512 512 32
#python3 ../gen.py 512 512 128
#python3 ../gen.py 512 512 512

##
## For testing mode
##
if [[ $1 == "--test" ]]
then
    exit 0
fi

##
## For BCSR formatting
##
if [[ $1 == "--bcsr" ]]
then
    echo "Running BCSR formatting..."
    
    source bin/config.sh
    
    if [[ ! -d data/bcsr ]]
    then
        mkdir data/bcsr
    fi
    
    for mtx in data/*.mtx
    do
        input_mtx=`basename $mtx .mtx`
        
        for b in "${blocks[@]}"
        do
            echo "$input_mtx ${b}x${b}"
            build/fmtbcsr data/$input_mtx.mtx --output data/bcsr/"$input_mtx"_"$b".mtx --block $b
        done
    done
    
    exit 0
fi

cd data

rm -rf ./*

##
## Download real-world sources
##
wget https://suitesparse-collection-website.herokuapp.com/MM/HB/bcsstk13.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/HB/bcsstk17.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Williams/cant.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Williams/pdb1HYS.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Bova/rma10.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Bai/dw4096.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Williams/cop20k_A.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Bai/af23560.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/DNVS/x104.tar.gz

wget https://suitesparse-collection-website.herokuapp.com/MM/MaxPlanck/shallow_water1.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Um/2cubes_sphere.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Hamm/scircuit.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Williams/mac_econ_fwd500.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Williams/webbase-1M.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/GHS_psdef/hood.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/GHS_indef/bmw3_2.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/ATandT/pre2.tar.gz

wget https://suitesparse-collection-website.herokuapp.com/MM/Boeing/pwtk.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/GHS_psdef/crankseg_2.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Norris/torso1.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Bourchtein/atmosmodd.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/INPRO/msdoor.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Koutsovasilis/F1.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/ND/nd24k.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/GHS_psdef/inline_1.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/GHS_psdef/ldoor.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/vanHeukelum/cage14.tar.gz

for d in "${data[@]}"
do
    echo "Unzipping $d"
    tar -xvzf $d.tar.gz
    mv $d/$d.mtx .
    rm -rf ./$d
done

rm *.tar.gz

echo "Done!"

