#!/bin/bash
BENCH_NAME="all"
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
            ##
            ## COO
            ##
            echo "[Serial] coo --k $k"
            printf "COO Serial," >> $CSV_FILE
            $BIN/coo_serial_${O} data/$NAME.mtx --iters $iters --k $k >> $CSV_FILE
            
            ##
            ## CSR
            ##
            echo "[Serial] csr --k $k"
            printf "CSR Serial," >> $CSV_FILE
            $BIN/csr_serial_${O} data/$NAME.mtx --iters $iters --k $k >> $CSV_FILE
            
            ##
            ## ELL
            ##
            echo "[Serial] ell --k $k"
            printf "ELL Serial," >> $CSV_FILE
            $BIN/ell_serial_${O} data/$NAME.mtx --iters $iters --k $k >> $CSV_FILE
            
            ##
            ## BCSR
            ##
            for b in "${blocks[@]}"
            do
                echo "[Serial] bcsr --k $k ${b}x${b}"
                printf "BCSR Serial," >> $CSV_FILE
                $BIN/bcsr_serial_${O} data/$NAME.mtx --iters $iters --k $k --block $b >> $CSV_FILE
            done
            
        done #end K
    done # end O2
}

##
## Run the benchmark on all available data
##
for mtx in data/*.mtx
do
    input_mtx=`basename $mtx .mtx`
    
    echo ""
    echo "-----------------------------------------------"
    echo "MATRIX: $input_mtx"
    echo "-----------------------------------------------"
    
    run $input_mtx
done

