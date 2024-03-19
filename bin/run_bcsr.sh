#!/bin/bash
BENCH_NAME="bcsr"
ARCH_ID=$1

source bin/config.sh
source bin/lib.sh
setup

##
## Our run function
##
function run() {
    NAME=$1
    CSV_FILE=report/csv/$1_"$BENCH_NAME"_"$ARCH_ID".csv
    init_csv $CSV_FILE
    
    for k in "${k_loop[@]}"
    do
        for O in "${OLEVELS[@]}"
        do
            for b in "${blocks[@]}"
            do
                for t in "${threads[@]}"
                do
                    echo "[OMP] bcsr --k $k ${b}x${b} --threads $t"
                    printf "bcsr_omp_${O}," >> $CSV_FILE
                    $BIN/bcsr_omp_${O} data/$NAME.mtx --iters $iters --k $k --block $b --threads $t >> $CSV_FILE
                    
                    echo "[OMP Collapse] bcsr --k $k ${b}x${b} --threads $t"
                    printf "bcsr_omp_collapse_${O}," >> $CSV_FILE
                    $BIN/bcsr_omp_collapse_${O} data/$NAME.mtx --iters $iters --k $k --block $b --threads $t >> $CSV_FILE
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

