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
    printf "Name,GFLOPS,FOP Count,Avg Run Time (s),Format Time (s),Total Time (s)," >> $CSV_FILE
    printf "Verification,Iters,Block Row,Block Col,K-Bound,Threads,Rows,Cols,NNZ,NNZ*2," >> $CSV_FILE
    printf "Max Cols,Avg Cols,Variance,Std Deviation" >> $CSV_FILE
    echo "" >> $CSV_FILE
}

