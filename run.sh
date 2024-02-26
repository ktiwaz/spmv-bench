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

iters=1
blocks=(1 2 4 16)

##
## Function for running COO
##
function run_coo() {
    NAME=$1
    CSV_FILE=$2
    
    echo "coo_serial"
    printf "coo_serial," >> $CSV_FILE
    build/bin/coo_serial data/$NAME.mtx --iters $iters >> $CSV_FILE
    
    echo "coo_transpose_serial"
    printf "coo_transpose_serial," >> $CSV_FILE
    build/bin/coo_transpose_serial data/$NAME.mtx --iters $iters >> $CSV_FILE
    
    for t in "${blocks[@]}"
    do
        echo "coo_omp --threads $t"
        printf "coo_omp -t $NAME," >> $CSV_FILE
        build/bin/coo_omp data/$NAME.mtx --iters $iters --threads $t >> $CSV_FILE
        
        echo "coo_transpose_omp --threads $t"
        printf "coo_transpose_omp -t $NAME," >> $CSV_FILE
        build/bin/coo_transpose_omp data/$NAME.mtx --iters $iters --threads $t >> $CSV_FILE
        
        echo "coo_transpose_omp_omp --threads $t"
        printf "coo_transpose_omp_omp -t $NAME," >> $CSV_FILE
        build/bin/coo_transpose_omp_omp data/$NAME.mtx --iters $iters --threads $t >> $CSV_FILE
    done
    
    echo "coo_omp_gpu"
    printf "coo_omp_gpu," >> $CSV_FILE
    build/bin/coo_omp_gpu data/$NAME.mtx --iters $iters >> $CSV_FILE
    
    echo "coo_transpose_omp_gpu"
    printf "coo_transpose_omp_gpu," >> $CSV_FILE
    build/bin/coo_transpose_omp_gpu data/$NAME.mtx --iters $iters >> $CSV_FILE
}

##
## Function for running CSR
##
function run_csr() {
    NAME=$1
    CSV_FILE=$2
    
    echo "csr_serial"
    printf "csr_serial," >> $CSV_FILE
    build/bin/csr_serial data/$NAME.mtx --iters $iters >> $CSV_FILE
    
    echo "csr_transpose_serial"
    printf "csr_transpose_serial," >> $CSV_FILE
    build/bin/csr_transpose_serial data/$NAME.mtx --iters $iters >> $CSV_FILE
    
    for t in "${blocks[@]}"
    do
        echo "csr_omp --threads $t"
        printf "csr_omp -t $NAME," >> $CSV_FILE
        build/bin/csr_omp data/$NAME.mtx --iters $iters --threads $t >> $CSV_FILE
        
        echo "csr_transpose_omp --threads $t"
        printf "csr_transpose_omp -t $NAME," >> $CSV_FILE
        build/bin/csr_transpose_omp data/$NAME.mtx --iters $iters --threads $t >> $CSV_FILE
        
        echo "csr_transpose_omp_omp --threads $t"
        printf "csr_transpose_omp_omp -t $NAME," >> $CSV_FILE
        build/bin/csr_transpose_omp_omp data/$NAME.mtx --iters $iters --threads $t >> $CSV_FILE
    done
    
    echo "csr_omp_gpu"
    printf "csr_omp_gpu," >> $CSV_FILE
    build/bin/csr_omp_gpu data/$NAME.mtx --iters $iters >> $CSV_FILE
    
    echo "csr_transpose_omp_gpu"
    printf "csr_transpose_omp_gpu," >> $CSV_FILE
    build/bin/csr_transpose_omp_gpu data/$NAME.mtx --iters $iters >> $CSV_FILE
}

##
## Function for running ELL
##
function run_ell() {
    NAME=$1
    CSV_FILE=$2
    
    echo "ell_serial"
    printf "ell_serial," >> $CSV_FILE
    build/bin/ell_serial data/$NAME.mtx --iters $iters >> $CSV_FILE
    
    echo "ell_transpose_serial"
    printf "ell_transpose_serial," >> $CSV_FILE
    build/bin/ell_transpose_serial data/$NAME.mtx --iters $iters >> $CSV_FILE
    
    for t in "${blocks[@]}"
    do
        echo "ell_omp --threads $t"
        printf "ell_omp -t $t," >> $CSV_FILE
        build/bin/ell_omp data/$NAME.mtx --iters $iters --threads $t >> $CSV_FILE
        
        echo "ell_transpose_omp --threads $t"
        printf "ell_transpose_omp -t $NAME," >> $CSV_FILE
        build/bin/ell_transpose_omp data/$NAME.mtx --iters $iters --threads $t >> $CSV_FILE
        
        echo "ell_transpose_omp_omp --threads $t"
        printf "ell_transpose_omp_omp -t $NAME," >> $CSV_FILE
        build/bin/ell_transpose_omp_omp data/$NAME.mtx --iters $iters --threads $t >> $CSV_FILE
    done
    
    echo "ell_omp_gpu"
    printf "ell_omp_gpu," >> $CSV_FILE
    build/bin/ell_omp_gpu data/$NAME.mtx --iters $iters >> $CSV_FILE
    
    echo "ell_transpose_omp_gpu"
    printf "ell_transpose_omp_gpu," >> $CSV_FILE
    build/bin/ell_transpose_omp_gpu data/$NAME.mtx --iters $iters >> $CSV_FILE
}

##
## Function for running BCSR
##
function run_bcsr() {
    NAME=$1
    CSV_FILE=$2
    
    for b in "${blocks[@]}"
    do
        echo "bcsr_serial $b x $b"
        printf "bcsr_serial $b x $b," >> $CSV_FILE
        build/bin/bcsr_serial data/$NAME.mtx --iters $iters --block $b >> $CSV_FILE
        
        echo "bcsr_transpose_serial $b x $b"
        printf "bcsr_transpose_serial $b x $b," >> $CSV_FILE
        build/bin/bcsr_transpose_serial data/$NAME.mtx --iters $iters --block $b >> $CSV_FILE
    done
    
    for b in "${blocks[@]}"
    do
        for t in "${blocks[@]}"
        do
            echo "bcsr_omp $b x $b --threads $t"
            printf "bcsr_omp $b x $b -t $t," >> $CSV_FILE
            build/bin/bcsr_omp data/$NAME.mtx --iters $iters --threads $t --block $b >> $CSV_FILE
            
            echo "bcsr_transpose_omp $b x $b --threads $t"
            printf "bcsr_transpose_omp $b x $b -t $NAME," >> $CSV_FILE
            build/bin/bcsr_transpose_omp data/$NAME.mtx --iters $iters --threads $t --block $b >> $CSV_FILE
            
            echo "bcsr_transpose_omp_omp $b x $b --threads $t"
            printf "bcsr_transpose_omp_omp $b x $b -t $NAME," >> $CSV_FILE
            build/bin/bcsr_transpose_omp_omp data/$NAME.mtx --iters $iters --threads $t --block $b >> $CSV_FILE
        done
    done
    
    for b in "${blocks[@]}"
    do
        echo "bcsr_omp_gpu $b x $b"
        printf "bcsr_omp_gpu $b x $b," >> $CSV_FILE
        build/bin/bcsr_omp_gpu data/$NAME.mtx --iters $iters --block $b >> $CSV_FILE
        
        echo "bcsr_transpose_omp_gpu $b x $b"
        printf "bcsr_transpose_omp_gpu $b x $b," >> $CSV_FILE
        build/bin/bcsr_transpose_omp_gpu data/$NAME.mtx --iters $iters --block $b >> $CSV_FILE
    done
}

##
## $1 -> Input name
## $2 -> Test name
##
function run() {
    echo "Running: $1 (test: $2)"
    NAME=$1
    CSV_FILE=report/csv/$1.csv
    mode=$2
    
    printf "" > $CSV_FILE
    printf "Name,Avg Run Time (s),Format Time (s),Total Time (s),GFLOPS," >> $CSV_FILE
    printf "Verification,Iters,Block Row,Block Col,Threads,FOP Count,Rows,Cols,NNZ," >> $CSV_FILE
    printf "Max Cols,Avg Cols,Variance,Std Deviation" >> $CSV_FILE
    echo "" >> $CSV_FILE
    
    if [[ $mode == "coo" || $mode == "all" ]]
    then
        run_coo $NAME $CSV_FILE
    fi
    
    if [[ $mode == "csr" || $mode == "all" ]]
    then
        run_csr $NAME $CSV_FILE
    fi
    
    if [[ $mode == "ell" || $mode == "all" ]]
    then
        run_ell $NAME $CSV_FILE
    fi
    
    if [[ $mode == "bcsr" || $mode == "all" ]]
    then
        run_bcsr $NAME $CSV_FILE
    fi
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

mode="all"
if [[ $1 != "coo" && $1 != "csr" && $1 != "ell" && $1 != "bcsr" ]]
then
    mode="all"
else
    mode=$1
fi

for mtx in data/*.mtx
do
    input_mtx=`basename $mtx .mtx`
    run $input_mtx $mode
    post_process $input_mtx
done

echo "Done!!"

