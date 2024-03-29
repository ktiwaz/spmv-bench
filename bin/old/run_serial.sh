#!/bin/bash
BENCH_NAME="serial"
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
            echo "[Serial] coo --k $k"
            printf "coo_serial_${O}," >> $CSV_FILE
            $BIN/coo_serial_${O} data/$NAME.mtx --iters $iters --k $k >> $CSV_FILE
            
            echo "[Serial] csr --k $k"
            printf "csr_serial_${O}," >> $CSV_FILE
            $BIN/csr_serial_${O} data/$NAME.mtx --iters $iters --k $k >> $CSV_FILE
            
            echo "[Serial] ell --k $k"
            printf "ell_serial_${O}," >> $CSV_FILE
            $BIN/ell_serial_${O} data/$NAME.mtx --iters $iters --k $k >> $CSV_FILE
            
            for b in "${blocks[@]}"
            do
                echo "[Serial] bcsr --k $k ${b}x${b}"
                printf "bcsr_serial_${O}," >> $CSV_FILE
                $BIN/bcsr_serial_${O} data/$NAME.mtx --iters $iters --k $k --block $b >> $CSV_FILE
                
                echo "[Serial] bell --k $k ${b}"
                printf "bell_serial_${O}," >> $CSV_FILE
                $BIN/bell_serial_${O} data/$NAME.mtx --iters $iters --k $k --block $b >> $CSV_FILE
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

