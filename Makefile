CC=clang
CXX=clang++
CXXFLAGS=-Ilib build/libspmm.a -std=c++17

## Test binaries
TEST_SRC := $(wildcard test/*.cpp)
TEST_OUTPUT = build/test
TEST_BINS := $(patsubst test/%.cpp,$(TEST_OUTPUT)/%,$(TEST_SRC))

CSV=csv
BIN_OUTPUT = build/bin

BENCH_BINS=\
	$(BIN_OUTPUT)/csr_serial $(BIN_OUTPUT)/csr_omp \
	$(BIN_OUTPUT)/ell_serial $(BIN_OUTPUT)/ell_omp \
	$(BIN_OUTPUT)/bcsr_serial $(BIN_OUTPUT)/bcsr_omp

## The core
all: check_dir build/libspmm.a $(TEST_BINS) $(BENCH_BINS)

.PHONY: clean_dir
check_dir:
	if [ ! -d ./build ]; then mkdir build; fi; \
	if [ ! -d ./build/test ] ; then mkdir build/test; fi; \
	if [ ! -d ./$(BIN_OUTPUT) ] ; then mkdir $(BIN_OUTPUT); fi; \
	if [ ! -d ./$(CSV) ]; then mkdir $(CSV); fi;

##
## Build the static library
##
build/libspmm.a: build/spmm.o
	ar rvs build/libspmm.a build/spmm.o

build/spmm.o: lib/spmm.cpp
	$(CXX) lib/spmm.cpp -c -o build/spmm.o -O2
	
##
## Build the test binaries
##
$(TEST_OUTPUT)/%: test/%.cpp build/libspmm.a
	$(CXX) $< -o $@ $(CXXFLAGS) -O2
	
########################################################################

##
## Builds the CSR executables
##
$(BIN_OUTPUT)/csr_serial: bin/csr/csr_serial.cpp build/libspmm.a
	$(CXX) bin/csr/csr_serial.cpp build/libspmm.a -o $(BIN_OUTPUT)/csr_serial $(CXXFLAGS) -O2 -march=native

$(BIN_OUTPUT)/csr_omp: bin/csr/csr_omp.cpp build/libspmm.a
	$(CXX) bin/csr/csr_omp.cpp build/libspmm.a -o $(BIN_OUTPUT)/csr_omp $(CXXFLAGS) -O2 -march=native -fopenmp

##
## Builds the ELL executables
##
$(BIN_OUTPUT)/ell_serial: bin/ell/ell_serial.cpp build/libspmm.a
	$(CXX) bin/ell/ell_serial.cpp build/libspmm.a -o $(BIN_OUTPUT)/ell_serial $(CXXFLAGS) -O2 -march=native

$(BIN_OUTPUT)/ell_omp: bin/ell/ell_omp.cpp build/libspmm.a
	$(CXX) bin/ell/ell_omp.cpp build/libspmm.a -o $(BIN_OUTPUT)/ell_omp $(CXXFLAGS) -O2 -march=native -fopenmp
	
##
## Builds the BCSR executables
##
$(BIN_OUTPUT)/bcsr_serial: bin/bcsr/bcsr_serial.cpp build/libspmm.a
	$(CXX) bin/bcsr/bcsr_serial.cpp build/libspmm.a -o $(BIN_OUTPUT)/bcsr_serial $(CXXFLAGS) -O2 -march=native

$(BIN_OUTPUT)/bcsr_omp: bin/bcsr/bcsr_omp.cpp build/libspmm.a
	$(CXX) bin/bcsr/bcsr_omp.cpp build/libspmm.a -o $(BIN_OUTPUT)/bcsr_omp $(CXXFLAGS) -O2 -march=native -fopenmp

########################################################################

##
## Clean target
##
.PHONY: clean
clean:
	rm -rf build/*

########################################################################
##
## Runs the benchmarks
##
RUN_COUNT=5

run: $(BENCH_BINS) run_bcsstk17
include run.mk

########################################################################

##
## Runs the print targets
##
.PHONY: run_print
run_print: $(TEST_BINS)
	$(TEST_OUTPUT)/print test_rank2.mtx; \
	printf "\n\n"; \
	$(TEST_OUTPUT)/print_csr test_rank2.mtx; \
	printf "\n\n"; \
	$(TEST_OUTPUT)/print_ell test_rank2.mtx; \
	printf "\n\n"; \
	$(TEST_OUTPUT)/print_bcsr test_rank2.mtx 2 2; \
	printf "\n\n";
	$(TEST_OUTPUT)/print_bcsr test_rank2.mtx 4 4;

