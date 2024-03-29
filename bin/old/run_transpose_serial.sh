#!/bin/bash
BENCH_NAME="transpose_serial"
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
            echo "[Transpose Serial] coo --k $k"
            printf "coo_transpose_serial_${O}," >> $CSV_FILE
            $BIN/coo_transpose_serial_${O} data/$NAME.mtx --iters $iters --k $k >> $CSV_FILE
            
            echo "[Transpose Serial] csr --k $k"
            printf "csr_transpose_serial_${O}," >> $CSV_FILE
            $BIN/csr_transpose_serial_${O} data/$NAME.mtx --iters $iters --k $k >> $CSV_FILE
            
            echo "[Transpose Serial] ell --k $k"
            printf "ell_transpose_serial_${O}," >> $CSV_FILE
            $BIN/ell_transpose_serial_${O} data/$NAME.mtx --iters $iters --k $k >> $CSV_FILE
            
            for b in "${blocks[@]}"
            do
                echo "[Transpose Serial] bcsr --k $k ${b}x${b}"
                printf "bcsr_transpose_serial_${O}," >> $CSV_FILE
                $BIN/bcsr_transpose_serial_${O} data/$NAME.mtx --iters $iters --k $k --block $b >> $CSV_FILE
                
                echo "[Transpose Serial] bell --k $k ${b}"
                printf "bell_transpose_serial_${O}," >> $CSV_FILE
                $BIN/bell_transpose_serial_${O} data/$NAME.mtx --iters $iters --k $k --block $b >> $CSV_FILE
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

