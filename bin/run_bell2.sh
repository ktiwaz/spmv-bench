#!/bin/bash
BENCH_NAME="bell"
ARCH_ID=$1

source bin/config.sh
source bin/lib.sh
setup

##
## Our run function
##
function run() {
    NAME=$1
    CSV_FILE=report/csv/$1_"$BENCH_NAME"_"$ARCH_ID"_transpose.csv
    init_csv $CSV_FILE
    
    for k in "${k_loop[@]}"
    do
        for O in "${OLEVELS[@]}"
        do
            for b in "${blocks[@]}"
            do
                for t in "${threads[@]}"
                do
                    echo "[Transpose OMP] bell --k $k ${b} --threads $t"
                    printf "bell_transpose_omp_${O}," >> $CSV_FILE
                    $BIN/bell_transpose_omp_${O} data/$NAME.mtx --iters $iters --k $k --block $b --threads $t >> $CSV_FILE
                    
                    echo "[Transpose OMP Collapse] bell --k $k ${b} --threads $t"
                    printf "bell_transpose_omp_collapse_${O}," >> $CSV_FILE
                    $BIN/bell_transpose_omp_collapse_${O} data/$NAME.mtx --iters $iters --k $k --block $b --threads $t >> $CSV_FILE
                    
                    echo "[Transpose OMPxOMP] bell --k $k ${b} --threads $t"
                    printf "bell_transpose_omp_omp_${O}," >> $CSV_FILE
                    $BIN/bell_transpose_omp_omp_${O} data/$NAME.mtx --iters $iters --k $k --block $b --threads $t >> $CSV_FILE
                done
            done
        done
    done
}

##
## Run the benchmark on all available data
##
for mtx in data/*.mtx
do
    input_mtx=`basename $mtx .mtx`
    run $input_mtx
done

