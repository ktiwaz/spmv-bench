##
## Real world data- BCSSTK17
##
.PHONY: run_bcsstk17
run_bcsstk17:
	echo "Test Name,Time (s)" > $(CSV)/bcsstk17.csv
	
	$(BIN_OUTPUT)/csr_serial $(RUN_COUNT) data/bcsstk17.mtx >> $(CSV)/bcsstk17.csv
	$(BIN_OUTPUT)/csr_omp $(RUN_COUNT) data/bcsstk17.mtx >> $(CSV)/bcsstk17.csv
	
	$(BIN_OUTPUT)/ell_serial $(RUN_COUNT) data/bcsstk17.mtx >> $(CSV)/bcsstk17.csv
	$(BIN_OUTPUT)/ell_omp $(RUN_COUNT) data/bcsstk17.mtx >> $(CSV)/bcsstk17.csv
	
	for blk in 1 2 4 8 16 32; do \
	    $(BIN_OUTPUT)/bcsr_serial $(RUN_COUNT) data/bcsstk17.mtx $$blk $$blk >> $(CSV)/bcsstk17.csv; \
	    $(BIN_OUTPUT)/bcsr_omp $(RUN_COUNT) data/bcsstk17.mtx $$blk $$blk >> $(CSV)/bcsstk17.csv; \
	done 
