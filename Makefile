CC=gcc
CXX=g++
CXXFLAGS=-Ilib build/libspmm.a -std=c++17
NVFLAGS=-Ilib build/libspmm.a

## Test binaries
TEST_SRC := $(wildcard test/*.cpp)
TEST_OUTPUT = build/test
TEST_BINS := $(patsubst test/%.cpp,$(TEST_OUTPUT)/%,$(TEST_SRC))

CSV=csv
BIN_OUTPUT = build/bin

BENCH_BINS=\
	$(BIN_OUTPUT)/coo_serial $(BIN_OUTPUT)/coo_omp \
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

build/spmm.o: lib/spmm.cpp lib/spmm.h lib/csr.h lib/ell.h lib/bell.h lib/bcsr.h
	$(CXX) lib/spmm.cpp -c -o build/spmm.o -O2 -fPIE
	
##
## Build the test binaries
##
CXX16=/opt/llvm/llvm-16.x-install/bin/clang++

$(TEST_OUTPUT)/%: test/%.cpp build/libspmm.a
	$(CXX) $< -o $@ $(CXXFLAGS) -O2 -fopenmp

# GPU test binaries
gpu_test: \
	$(TEST_OUTPUT)/print_csr2 $(TEST_OUTPUT)/print_coo2 \
	$(TEST_OUTPUT)/print_ell2 $(TEST_OUTPUT)/print_bcsr2 \
	$(TEST_OUTPUT)/omp_test1 $(TEST_OUTPUT)/gpu_csr1 $(TEST_OUTPUT)/gpu_test1

$(TEST_OUTPUT)/print_csr2: test/gpu/print_csr2.cpp build/libspmm.a
	$(CXX16) test/gpu/print_csr2.cpp -o $(TEST_OUTPUT)/print_csr2 $(CXXFLAGS) -O2 \
		-fopenmp-targets=nvptx64 -fopenmp

$(TEST_OUTPUT)/print_coo2: test/gpu/print_coo2.cpp build/libspmm.a
	$(CXX16) test/gpu/print_coo2.cpp -o $(TEST_OUTPUT)/print_coo2 $(CXXFLAGS) -O2 \
		-fopenmp-targets=nvptx64 -fopenmp

$(TEST_OUTPUT)/print_ell2: test/gpu/print_ell2.cpp build/libspmm.a
	$(CXX16) test/gpu/print_ell2.cpp -o $(TEST_OUTPUT)/print_ell2 $(CXXFLAGS) -O2 \
		-fopenmp-targets=nvptx64 -fopenmp

$(TEST_OUTPUT)/print_bcsr2: test/gpu/print_bcsr2.cpp build/libspmm.a
	$(CXX16) test/gpu/print_bcsr2.cpp -o $(TEST_OUTPUT)/print_bcsr2 $(CXXFLAGS) -O2 \
		-fopenmp-targets=nvptx64 -fopenmp

$(TEST_OUTPUT)/omp_test1: test/gpu/omp_test1.cpp build/libspmm.a
	$(CXX16) test/gpu/omp_test1.cpp -o $(TEST_OUTPUT)/omp_test1 $(CXXFLAGS) -O2 \
		-fopenmp-targets=nvptx64 -fopenmp

$(TEST_OUTPUT)/gpu_csr1: test/gpu/gpu_csr1.cu build/libspmm.a
	nvcc test/gpu/gpu_csr1.cu -o $(TEST_OUTPUT)/gpu_csr1 $(NVFLAGS) -O2 -g

$(TEST_OUTPUT)/gpu_test1: test/gpu/gpu_test1.cu build/libspmm.a
	nvcc test/gpu/gpu_test1.cu -o $(TEST_OUTPUT)/gpu_test1 $(NVFLAGS) -O2 -g
	
########################################################################
BASE=bin/main.cpp build/libspmm.a
DEPS=build/libspmm.a bin/main.cpp

##
## Builds the COO executables
##
$(BIN_OUTPUT)/coo_serial: bin/coo/coo_serial.cpp $(DEPS)
	$(CXX) -Ibin/coo $(BASE) bin/coo/coo_serial.cpp -o $(BIN_OUTPUT)/coo_serial $(CXXFLAGS) -O2 -march=native

$(BIN_OUTPUT)/coo_omp: bin/coo/coo_omp.cpp $(DEPS)
	$(CXX) -Ibin/coo $(BASE) bin/coo/coo_omp.cpp -o $(BIN_OUTPUT)/coo_omp $(CXXFLAGS) -O2 -march=native -fopenmp

##
## Builds the CSR executables
##
$(BIN_OUTPUT)/csr_serial: bin/csr/csr_serial.cpp $(DEPS)
	$(CXX) -Ibin/csr $(BASE) bin/csr/csr_serial.cpp -o $(BIN_OUTPUT)/csr_serial $(CXXFLAGS) -O2 -march=native

$(BIN_OUTPUT)/csr_omp: bin/csr/csr_omp.cpp $(DEPS)
	$(CXX) -Ibin/csr $(BASE) bin/csr/csr_omp.cpp -o $(BIN_OUTPUT)/csr_omp $(CXXFLAGS) -O2 -march=native -fopenmp

##
## Builds the ELL executables
##
$(BIN_OUTPUT)/ell_serial: bin/ell/ell_serial.cpp $(DEPS)
	$(CXX) -Ibin/ell $(BASE) bin/ell/ell_serial.cpp -o $(BIN_OUTPUT)/ell_serial $(CXXFLAGS) -O2 -march=native

$(BIN_OUTPUT)/ell_omp: bin/ell/ell_omp.cpp $(DEPS)
	$(CXX) -Ibin/ell $(BASE) bin/ell/ell_omp.cpp -o $(BIN_OUTPUT)/ell_omp $(CXXFLAGS) -O2 -march=native -fopenmp
	
##
## Builds the BCSR executables
##
$(BIN_OUTPUT)/bcsr_serial: bin/bcsr/bcsr_serial.cpp $(DEPS)
	$(CXX) -Ibin/bcsr $(BASE) bin/bcsr/bcsr_serial.cpp -o $(BIN_OUTPUT)/bcsr_serial $(CXXFLAGS) -O2 -march=native

$(BIN_OUTPUT)/bcsr_omp: bin/bcsr/bcsr_omp.cpp $(DEPS)
	$(CXX)  -Ibin/bcsr $(BASE) bin/bcsr/bcsr_omp.cpp -o $(BIN_OUTPUT)/bcsr_omp $(CXXFLAGS) -O2 -march=native -fopenmp

########################################################################

##
## Clean target
##
.PHONY: clean
clean:
	rm -rf build/*


