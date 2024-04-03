#!/bin/bash
BENCH_NAME="coo_cusparse"
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
    
    echo "[CUSPARSE] coo"
    printf "COO cuSparse,O2," >> $CSV_FILE
    $BIN/coo_cusparse data/$NAME.mtx --iters $iters >> $CSV_FILE
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

