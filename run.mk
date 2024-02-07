##
## Synthetic matrix generation
##
.PHONY: gen_matrix
gen_matrix:
	if [ ! -d ./build/gen/512 ] ; then mkdir -p ./build/gen/512; fi; \
	if [ ! -d ./build/gen/1024 ] ; then mkdir -p ./build/gen/1024; fi; \
	cd ./build/gen/512; \
	python3 ../../../gen.py 512 512 8; \
	python3 ../../../gen.py 512 512 16; \
	python3 ../../../gen.py 512 512 32; \
	python3 ../../../gen.py 512 512 64; \
	python3 ../../../gen.py 512 512 128; \
	python3 ../../../gen.py 512 512 512; \
	cd ../1024; \
	python3 ../../../gen.py 1024 1024 8; \
	python3 ../../../gen.py 1024 1024 16; \
	python3 ../../../gen.py 1024 1024 32; \
	python3 ../../../gen.py 1024 1024 64; \
	python3 ../../../gen.py 1024 1024 512; \
	python3 ../../../gen.py 1024 1024 1024;

##
## Benchmark on synthetic matrix data
##
.PHONY: run_synth
run_synth: gen_matrix
	for mtx in "512x8" "512x16" "512x32" "512x64" "512x128" "512x512"; do \
		echo "Test Name,Time (s)" > $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/csr_serial $(RUN_COUNT) build/gen/512/$$mtx.mtx >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/csr_omp $(RUN_COUNT) build/gen/512/$$mtx.mtx >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/ell_serial $(RUN_COUNT) build/gen/512/$$mtx.mtx >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/ell_omp $(RUN_COUNT) build/gen/512/$$mtx.mtx >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_serial $(RUN_COUNT) build/gen/512/$$mtx.mtx 1 1 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_omp $(RUN_COUNT) build/gen/512/$$mtx.mtx 1 1 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_serial $(RUN_COUNT) build/gen/512/$$mtx.mtx 2 2 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_omp $(RUN_COUNT) build/gen/512/$$mtx.mtx 2 2 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_serial $(RUN_COUNT) build/gen/512/$$mtx.mtx 4 4 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_omp $(RUN_COUNT) build/gen/512/$$mtx.mtx 4 4 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_serial $(RUN_COUNT) build/gen/512/$$mtx.mtx 8 8 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_omp $(RUN_COUNT) build/gen/512/$$mtx.mtx 8 8 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_serial $(RUN_COUNT) build/gen/512/$$mtx.mtx 16 16 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_omp $(RUN_COUNT) build/gen/512/$$mtx.mtx 16 16 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_serial $(RUN_COUNT) build/gen/512/$$mtx.mtx 32 32 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_omp $(RUN_COUNT) build/gen/512/$$mtx.mtx 32 32 >> $(CSV)/$$mtx.csv; \
	done ;
	for mtx in "1024x8" "1024x16" "1024x32" "1024x64" "1024x512" "1024x1024"; do \
		echo "Test Name,Time (s)" > $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/csr_serial $(RUN_COUNT) build/gen/1024/$$mtx.mtx >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/csr_omp $(RUN_COUNT) build/gen/1024/$$mtx.mtx >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/ell_serial $(RUN_COUNT) build/gen/1024/$$mtx.mtx >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/ell_omp $(RUN_COUNT) build/gen/1024/$$mtx.mtx >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_serial $(RUN_COUNT) build/gen/1024/$$mtx.mtx 1 1 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_omp $(RUN_COUNT) build/gen/1024/$$mtx.mtx 1 1 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_serial $(RUN_COUNT) build/gen/1024/$$mtx.mtx 2 2 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_omp $(RUN_COUNT) build/gen/1024/$$mtx.mtx 2 2 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_serial $(RUN_COUNT) build/gen/1024/$$mtx.mtx 4 4 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_omp $(RUN_COUNT) build/gen/1024/$$mtx.mtx 4 4 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_serial $(RUN_COUNT) build/gen/1024/$$mtx.mtx 8 8 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_omp $(RUN_COUNT) build/gen/1024/$$mtx.mtx 8 8 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_serial $(RUN_COUNT) build/gen/1024/$$mtx.mtx 16 16 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_omp $(RUN_COUNT) build/gen/1024/$$mtx.mtx 16 16 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_serial $(RUN_COUNT) build/gen/1024/$$mtx.mtx 32 32 >> $(CSV)/$$mtx.csv; \
		$(BIN_OUTPUT)/bcsr_omp $(RUN_COUNT) build/gen/1024/$$mtx.mtx 32 32 >> $(CSV)/$$mtx.csv; \
	done ;

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
	
