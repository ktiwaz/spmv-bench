#!/bin/bash

if [ ! -d ./csv ]
then
    mkdir csv
fi

##
## $1 -> Input name
##
function run() {
    echo "Running: $1"
    CSV_FILE=csv/$1.csv
    blocks=(1 2 4 8 16 32)
    iters=1
    
    printf "" > $CSV_FILE
    printf "Name,Avg Run Time (s),Format Time (s),Total Time (s)," >> $CSV_FILE
    printf "Verification,Iters,Block Row,Block Col,Threads,Rows,Cols,NNZ,Max Cols,Avg Cols" >> $CSV_FILE
    echo "" >> $CSV_FILE
    
    ##
    ## CSR
    ##
    echo "csr_serial"
    printf "csr_serial," >> $CSV_FILE
    build/bin/csr_serial data/$1.mtx --iters $iters >> $CSV_FILE
    
    for t in "${blocks[@]}"
    do
        echo "csr_omp --threads $t"
        printf "csr_omp," >> $CSV_FILE
        build/bin/csr_omp data/$1.mtx --iters $iters --threads $t >> $CSV_FILE
    done
    
    ##
    ## ELL
    ##
    echo "ell_serial"
    printf "ell_serial," >> $CSV_FILE
    build/bin/ell_serial data/$1.mtx --iters $iters >> $CSV_FILE
    
    for t in "${blocks[@]}"
    do
        echo "ell_omp --threads $t"
        printf "ell_omp," >> $CSV_FILE
        build/bin/ell_omp data/$1.mtx --iters $iters --threads $t >> $CSV_FILE
    done
    
    ##
    ## BCSR
    ##
    for b in "${blocks[@]}"
    do
        echo "bcsr_serial $b x $b"
        printf "bcsr_serial," >> $CSV_FILE
        build/bin/bcsr_serial data/$1.mtx --iters $iters --block $b >> $CSV_FILE
    done
    
    for b in "${blocks[@]}"
    do
        for t in "${blocks[@]}"
        do
            echo "bcsr_omp $b x $b --threads $t"
            printf "bcsr_omp," >> $CSV_FILE
            build/bin/bcsr_omp data/$1.mtx --iters $iters --threads $t --block $b >> $CSV_FILE
        done
    done
}

run "bcsstk17"

