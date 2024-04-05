#!/bin/bash
BENCH_NAME="coo_omp_threads"
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
    
    for O in "${OLEVELS[@]}"
    do
        for k in "${k_loop[@]}"
        do
            echo "[THREADS] coo --k $k --thread-list $tlist"
            printf "COO THREADS,${O}," >> $CSV_FILE
            $BIN/coo_omp_${O} data/$NAME.mtx --iters $iters_tlist --k $k --thread-list $tlist >> $CSV_FILE
        done
    done
}

##
## Run the benchmark on all available data
##
for mtx in data/*.mtx
do
    input_mtx=`basename $mtx .mtx`
    
    echo ""
    echo "-----------$input_mtx--------------------"
    run $input_mtx
done

