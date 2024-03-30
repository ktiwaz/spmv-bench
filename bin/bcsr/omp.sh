#!/bin/bash
BENCH_NAME="bcsr_omp"
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
        for t in "${threads[@]}"
        do
            for k in "${k_loop[@]}"
            do
                for b in "${blocks[@]}"
                do
                    echo "[OMP] bcsr --k $k --threads $t ${b}x${b}"
                    printf "BCSR OMP,${O}," >> $CSV_FILE
                    $BIN/bcsr_omp_${O} data/$NAME.mtx --output data/bcsr/"$NAME"_"$b".mtx --iters $iters --k $k --threads $t --block $b >> $CSV_FILE
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
    
    echo ""
    echo "-----------$input_mtx--------------------"
    run $input_mtx
done

