#!/bin/bash
BENCH_NAME="omp_gpu"
ARCH_ID=$1

source bin/config.sh
source bin/lib.sh
setup

##
## Our run function
##
function run() {
    NAME=$1
    CSV_FILE=report/csv/$1_"$BENCH_NAME"_"$ARCH_ID"_gpu.csv
    init_csv $CSV_FILE
    
    for k in "${k_loop[@]}"
    do
        for O in "${OLEVELS[@]}"
        do
            echo "[OMPxGPU] coo --k $k"
            printf "coo_omp_gpu_${O}," >> $CSV_FILE
            $BIN/coo_omp_gpu_${O} data/$NAME.mtx --iters $iters --k $k >> $CSV_FILE
            
            echo "[OMPxGPU] csr --k $k"
            printf "csr_omp_gpu_${O}," >> $CSV_FILE
            $BIN/csr_omp_gpu_${O} data/$NAME.mtx --iters $iters --k $k >> $CSV_FILE
            
            echo "[OMPxGPU] ell --k $k"
            printf "ell_omp_gpu_${O}," >> $CSV_FILE
            $BIN/ell_omp_gpu_${O} data/$NAME.mtx --iters $iters --k $k >> $CSV_FILE
            
            for b in "${blocks[@]}"
            do
                echo "[OMPxGPU] bcsr --k $k ${b}x${b}"
                printf "bcsr_omp_gpu_${O}," >> $CSV_FILE
                $BIN/bcsr_omp_gpu_${O} data/$NAME.mtx --iters $iters --k $k --block $b >> $CSV_FILE
                
                echo "[OMPxGPU] bell --k $k ${b}"
                printf "bell_omp_gpu_${O}," >> $CSV_FILE
                $BIN/bell_omp_gpu_${O} data/$NAME.mtx --iters $iters --k $k --block $b >> $CSV_FILE
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

