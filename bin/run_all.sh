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
        ##
        ## COO
        ##
        echo "[Serial] coo"
        printf "COO Serial,${O}," >> $CSV_FILE
        $BIN/coo_serial_${O} data/$NAME.mtx --iters $iters >> $CSV_FILE

        for t in "${threads[@]}"
        do
            echo "[OMP] coo --threads $t"
            printf "COO OMP,${O}," >> $CSV_FILE
            $BIN/coo_omp_${O} data/$NAME.mtx --iters $iters --threads $t >> $CSV_FILE
        done
            
        ##
        ## CSR
        ##
        echo "[Serial] csr"
        printf "CSR Serial,${O}," >> $CSV_FILE
        $BIN/csr_serial_${O} data/$NAME.mtx --iters $iters >> $CSV_FILE

        for t in "${threads[@]}"
        do
                echo "[OMP] csr --threads $t"
                printf "CSR OMP,${O}," >> $CSV_FILE
                $BIN/csr_omp_${O} data/$NAME.mtx --iters $iters --threads $t >> $CSV_FILE
        done
        
        ##
        ## ELL
        ##
        echo "[Serial] ell"
        printf "ELL Serial,${O}," >> $CSV_FILE
        $BIN/ell_serial_${O} data/$NAME.mtx --iters $iters >> $CSV_FILE

        for t in "${threads[@]}"
        do
                echo "[OMP] ell --threads $t"
                printf "ELL OMP,${O}," >> $CSV_FILE
                $BIN/ell_omp_${O} data/$NAME.mtx --iters $iters --threads $t >> $CSV_FILE
        done
        
        ##
        ## BCSR
        ##
        # for b in "${blocks[@]}"
        # do
        #     echo "[Serial] bcsr --k $k ${b}x${b}"
        #     printf "BCSR Serial," >> $CSV_FILE
        #     $BIN/bcsr_serial_${O} data/$NAME.mtx --iters $iters --k $k --block $b >> $CSV_FILE
        # done
    
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

