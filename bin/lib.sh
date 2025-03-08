##
## A setup function for setting the environment
##
function setup() {
    ##
    ## Check the folders
    ##
    if [ ! -d ./report ]
    then
        mkdir report
        mkdir report/csv
        mkdir report/images
    fi

    if [ ! -d ./report/csv ]
    then
        mkdir report/csv
    fi

    if [ ! -d ./report/images ]
    then
        mkdir report/images
    fi
}

##
## A function for setting up a CSV file
##
function init_csv() {
    CSV_FILE=$1
    printf "" > $CSV_FILE
    printf "Name,-O,Matrix,FLOPS,MFLOPS,GFLOPS,FOP Count,Avg Run Time (s),Format Time (s),Total Time (s)," >> $CSV_FILE
    printf "Verification,Iters,Block Row,Block Col,Threads,Rows,Cols,NNZ,NNZ*2," >> $CSV_FILE
    printf "Max Cols,Avg Cols,Col Ratio,Variance,Std Deviation" >> $CSV_FILE
    printf ",CPU Cycles,Instructions,Cache References,Cache Misses,Cache Miss Rate,Branch Instructions,Branch Misses,Branch Miss Rate,DTLB Loads,DTLB Misses,DTLB Miss Rate,Page Faults,Page Fault Rate,L1D Loads,L1D Misses,L1D Miss Rate,L1I Loads,L1I Misses,L1I Miss Rate," >> $CSV_FILE
    echo "" >> $CSV_FILE
}

