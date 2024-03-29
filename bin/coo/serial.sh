#!/bin/bash
BENCH_NAME="coo_serial"
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
            echo "[Serial] coo --k $k"
            printf "COO Serial,${O}," >> $CSV_FILE
            $BIN/coo_serial_${O} data/$NAME.mtx --iters $iters --k $k >> $CSV_FILE
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

