#!/bin/bash

if [ ! -d ./report ]
then
    mkdir report
    mkdir report/csv
    mkdir report/images
fi

if [ ! -d ./report/csv ]
then
    mkdir report/csv
fi

if [ ! -d ./report/images ]
then
    mkdir report/images
fi

##
## $1 -> Input name
##
function run() {
    echo "Running: $1"
    CSV_FILE=report/csv/$1.csv
    blocks=(1 2 4 8 16 32)
    iters=1
    
    printf "" > $CSV_FILE
    printf "Name,Avg Run Time (s),Format Time (s),Total Time (s),GFLOPS," >> $CSV_FILE
    printf "Verification,Iters,Block Row,Block Col,Threads,FOP Count,Rows,Cols,NNZ," >> $CSV_FILE
    printf "Max Cols,Avg Cols,Variance,Std Deviation" >> $CSV_FILE
    echo "" >> $CSV_FILE
    
    ##
    ## COO
    ##
    echo "coo_serial"
    printf "coo_serial," >> $CSV_FILE
    build/bin/coo_serial data/$1.mtx --iters $iters >> $CSV_FILE
    exit
    
    for t in "${blocks[@]}"
    do
        echo "coo_omp --threads $t"
        printf "coo_omp -t $1," >> $CSV_FILE
        build/bin/coo_omp data/$1.mtx --iters $iters --threads $t >> $CSV_FILE
    done
    
    echo "coo_omp_gpu"
    printf "coo_omp_gpu," >> $CSV_FILE
    build/bin/coo_omp_gpu data/$1.mtx --iters $iters >> $CSV_FILE
    
    ##
    ## CSR
    ##
    echo "csr_serial"
    printf "csr_serial," >> $CSV_FILE
    build/bin/csr_serial data/$1.mtx --iters $iters >> $CSV_FILE
    
    for t in "${blocks[@]}"
    do
        echo "csr_omp --threads $t"
        printf "csr_omp -t $1," >> $CSV_FILE
        build/bin/csr_omp data/$1.mtx --iters $iters --threads $t >> $CSV_FILE
    done
    
    echo "csr_omp_gpu"
    printf "csr_omp_gpu," >> $CSV_FILE
    build/bin/csr_omp_gpu data/$1.mtx --iters $iters >> $CSV_FILE
    
    ##
    ## ELL
    ##
    echo "ell_serial"
    printf "ell_serial," >> $CSV_FILE
    build/bin/ell_serial data/$1.mtx --iters $iters >> $CSV_FILE
    
    for t in "${blocks[@]}"
    do
        echo "ell_omp --threads $t"
        printf "ell_omp -t $t," >> $CSV_FILE
        build/bin/ell_omp data/$1.mtx --iters $iters --threads $t >> $CSV_FILE
    done
    
    echo "ell_omp_gpu"
    printf "ell_omp_gpu," >> $CSV_FILE
    build/bin/ell_omp_gpu data/$1.mtx --iters $iters >> $CSV_FILE
    
    ##
    ## BCSR
    ##
    for b in "${blocks[@]}"
    do
        echo "bcsr_serial $b x $b"
        printf "bcsr_serial $b x $b," >> $CSV_FILE
        build/bin/bcsr_serial data/$1.mtx --iters $iters --block $b >> $CSV_FILE
    done
    
    for b in "${blocks[@]}"
    do
        for t in "${blocks[@]}"
        do
            echo "bcsr_omp $b x $b --threads $t"
            printf "bcsr_omp $b x $b -t $t," >> $CSV_FILE
            build/bin/bcsr_omp data/$1.mtx --iters $iters --threads $t --block $b >> $CSV_FILE
        done
    done
    
    for b in "${blocks[@]}"
    do
        echo "bcsr_omp_gpu $b x $b"
        printf "bcsr_omp_gpu $b x $b," >> $CSV_FILE
        build/bin/bcsr_omp_gpu data/$1.mtx --iters $iters --block $b >> $CSV_FILE
    done
}

function post_process() {
    ./plot.sh "$1" "csv"
    python3 process.py "$1"
    for f in report/csv/$1/*.csv
    do
        input_csv=`basename $f .csv`
        ./plot.sh $input_csv "csv/$1"
    done
}

for mtx in data/*.mtx
do
    input_mtx=`basename $mtx .mtx`
    run $input_mtx
    post_process $input_mtx
done

exit

##
## 512 synthetic matrix
##
run "512x8"
post_process "512x8"

if [[ $1 == "--test" ]]
then
    echo "Test mode- exiting now."
    exit 0
fi

run "512x16"
post_process "512x16"

run "512x32"
post_process "512x32"

run "512x128"
post_process "512x128"

run "512x512"
post_process "512x512"

##
## 1024 synthetic matrices
##
run "1024x8"
post_process "1024x8"

run "1024x16"
post_process "1024x16"

run "1024x32"
post_process "1024x32"

run "1024x512"
post_process "1024x512"

run "1024x1024"
post_process "1024x1024"

##
## Real world matrices
##
run "bcsstk17"
post_process "bcsstk17"

run "cant"
post_process "cant"

run "pdb1HYS"
post_process "pdb1HYS"

run "rma10"
post_process "rma10"

