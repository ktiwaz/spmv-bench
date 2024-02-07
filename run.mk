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

##
## Real world data- cant
##
.PHONY: run_cant
run_cant:
	echo "Test Name,Time (s)" > $(CSV)/cant.csv
	
	$(BIN_OUTPUT)/csr_serial $(RUN_COUNT) data/cant.mtx >> $(CSV)/cant.csv
	$(BIN_OUTPUT)/csr_omp $(RUN_COUNT) data/cant.mtx >> $(CSV)/cant.csv
	
	$(BIN_OUTPUT)/ell_serial $(RUN_COUNT) data/cant.mtx >> $(CSV)/cant.csv
	$(BIN_OUTPUT)/ell_omp $(RUN_COUNT) data/cant.mtx >> $(CSV)/cant.csv
	
	for blk in 1 2 4 8 16 32; do \
	    $(BIN_OUTPUT)/bcsr_serial $(RUN_COUNT) data/cant.mtx $$blk $$blk >> $(CSV)/cant.csv; \
	    $(BIN_OUTPUT)/bcsr_omp $(RUN_COUNT) data/cant.mtx $$blk $$blk >> $(CSV)/cant.csv; \
	done 

##
## Real world data- pdb1HYS
##
.PHONY: run_pdb1HYS
run_pdb1HYS:
	echo "Test Name,Time (s)" > $(CSV)/pdb1HYS.csv
	
	$(BIN_OUTPUT)/csr_serial $(RUN_COUNT) data/pdb1HYS.mtx >> $(CSV)/pdb1HYS.csv
	$(BIN_OUTPUT)/csr_omp $(RUN_COUNT) data/pdb1HYS.mtx >> $(CSV)/pdb1HYS.csv
	
	$(BIN_OUTPUT)/ell_serial $(RUN_COUNT) data/pdb1HYS.mtx >> $(CSV)/pdb1HYS.csv
	$(BIN_OUTPUT)/ell_omp $(RUN_COUNT) data/pdb1HYS.mtx >> $(CSV)/pdb1HYS.csv
	
	for blk in 1 2 4 8 16 32; do \
	    $(BIN_OUTPUT)/bcsr_serial $(RUN_COUNT) data/pdb1HYS.mtx $$blk $$blk >> $(CSV)/pdb1HYS.csv; \
	    $(BIN_OUTPUT)/bcsr_omp $(RUN_COUNT) data/pdb1HYS.mtx $$blk $$blk >> $(CSV)/pdb1HYS.csv; \
	done 

##
## Real world data- rma10
##
.PHONY: run_rma10
run_rma10:
	echo "Test Name,Time (s)" > $(CSV)/rma10.csv
	
	$(BIN_OUTPUT)/csr_serial $(RUN_COUNT) data/rma10.mtx >> $(CSV)/rma10.csv
	$(BIN_OUTPUT)/csr_omp $(RUN_COUNT) data/rma10.mtx >> $(CSV)/rma10.csv
	
	$(BIN_OUTPUT)/ell_serial $(RUN_COUNT) data/rma10.mtx >> $(CSV)/rma10.csv
	$(BIN_OUTPUT)/ell_omp $(RUN_COUNT) data/rma10.mtx >> $(CSV)/rma10.csv
	
	for blk in 1 2 4 8 16 32; do \
	    $(BIN_OUTPUT)/bcsr_serial $(RUN_COUNT) data/rma10.mtx $$blk $$blk >> $(CSV)/rma10.csv; \
	    $(BIN_OUTPUT)/bcsr_omp $(RUN_COUNT) data/rma10.mtx $$blk $$blk >> $(CSV)/rma10.csv; \
	done 
	
