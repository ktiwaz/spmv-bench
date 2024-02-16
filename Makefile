CC=clang
CXX=clang++
CXX16=/opt/llvm/llvm-16.x-install/bin/clang++
CXXFLAGS=-Ilib build/libspmm.a -std=c++17 -O2 -march=native
OMPFLAGS=-fopenmp
GPUFLAGS=
NVFLAGS=-Ilib build/libspmm.a

ifeq ($(wildcard /opt/llvm/llvm-16.x-install/bin/clang++),)
    CXX=clang++
else
    CXX=$(CXX16)
    GPUFLAGS=-fopenmp-targets=nvptx64
endif

## Test binaries
TEST_SRC := $(wildcard test/*.cpp)
TEST_OUTPUT = build/test
TEST_BINS := $(patsubst test/%.cpp,$(TEST_OUTPUT)/%,$(TEST_SRC))

## GPU Test binaries
GPU_TEST_SRC := $(wildcard test/gpu/*.cpp)
GPU_TEST_OUTPUT = build/test/gpu
GPU_TEST_BINS := $(patsubst test/gpu/%.cpp,$(GPU_TEST_OUTPUT)/%,$(GPU_TEST_SRC))

BIN_OUTPUT = build/bin

BENCH_BINS=\
	$(BIN_OUTPUT)/coo_serial $(BIN_OUTPUT)/coo_omp $(BIN_OUTPUT)/coo_omp_gpu \
	$(BIN_OUTPUT)/csr_serial $(BIN_OUTPUT)/csr_omp $(BIN_OUTPUT)/csr_omp_gpu \
	$(BIN_OUTPUT)/ell_serial $(BIN_OUTPUT)/ell_omp $(BIN_OUTPUT)/ell_omp_gpu \
	$(BIN_OUTPUT)/bcsr_serial $(BIN_OUTPUT)/bcsr_omp $(BIN_OUTPUT)/bcsr_omp_gpu

## The core
all: check_dir build/libspmm.a $(TEST_BINS) $(GPU_TEST_BINS) $(BENCH_BINS)

.PHONY: clean_dir
check_dir:
	if [ ! -d ./build ]; then mkdir build; fi; \
	if [ ! -d ./build/test ] ; then mkdir build/test; fi; \
	if [ ! -d ./build/test/gpu ] ; then mkdir build/test/gpu; fi; \
	if [ ! -d ./$(BIN_OUTPUT) ] ; then mkdir $(BIN_OUTPUT); fi; \

##
## Build the static library
##
build/libspmm.a: build/spmm.o
	ar rvs build/libspmm.a build/spmm.o

build/spmm.o: lib/spmm.cpp lib/spmm.h lib/csr.h lib/ell.h lib/bell.h lib/bcsr.h
	$(CXX) lib/spmm.cpp -c -o build/spmm.o -O2 -fPIE -march=native
	
##
## Build the test binaries
##
$(TEST_OUTPUT)/%: test/%.cpp build/libspmm.a
	$(CXX) $< -o $@ $(CXXFLAGS) $(OMPFLAGS)

$(GPU_TEST_OUTPUT)/%: test/gpu/%.cpp build/libspmm.a
	$(CXX) $< -o $@ $(CXXFLAGS) $(OMPFLAGS) $(GPUFLAGS)
	
########################################################################
BASE=bin/main.cpp build/libspmm.a
DEPS=build/libspmm.a bin/main.cpp

##
## Builds the COO executables
##
$(BIN_OUTPUT)/coo_serial: bin/coo/coo_serial.cpp $(DEPS)
	$(CXX) -Ibin/coo $(BASE) bin/coo/coo_serial.cpp -o $(BIN_OUTPUT)/coo_serial $(CXXFLAGS)

$(BIN_OUTPUT)/coo_omp: bin/coo/coo_omp.cpp $(DEPS)
	$(CXX) -Ibin/coo $(BASE) bin/coo/coo_omp.cpp -o $(BIN_OUTPUT)/coo_omp $(CXXFLAGS) $(OMPFLAGS)
	
$(BIN_OUTPUT)/coo_omp_gpu: bin/coo/coo_omp_gpu.cpp $(DEPS)
	$(CXX) -Ibin/coo $(BASE) bin/coo/coo_omp_gpu.cpp -o $(BIN_OUTPUT)/coo_omp_gpu $(CXXFLAGS) $(OMPFLAGS) $(GPUFLAGS)

##
## Builds the CSR executables
##
$(BIN_OUTPUT)/csr_serial: bin/csr/csr_serial.cpp $(DEPS)
	$(CXX) -Ibin/csr $(BASE) bin/csr/csr_serial.cpp -o $(BIN_OUTPUT)/csr_serial $(CXXFLAGS)

$(BIN_OUTPUT)/csr_omp: bin/csr/csr_omp.cpp $(DEPS)
	$(CXX) -Ibin/csr $(BASE) bin/csr/csr_omp.cpp -o $(BIN_OUTPUT)/csr_omp $(CXXFLAGS) $(OMPFLAGS)

$(BIN_OUTPUT)/csr_omp_gpu: bin/csr/csr_omp_gpu.cpp $(DEPS)
	$(CXX) -Ibin/csr $(BASE) bin/csr/csr_omp_gpu.cpp -o $(BIN_OUTPUT)/csr_omp_gpu $(CXXFLAGS) $(OMPFLAGS) $(GPUFLAGS)

##
## Builds the ELL executables
##
$(BIN_OUTPUT)/ell_serial: bin/ell/ell_serial.cpp $(DEPS)
	$(CXX) -Ibin/ell $(BASE) bin/ell/ell_serial.cpp -o $(BIN_OUTPUT)/ell_serial $(CXXFLAGS)

$(BIN_OUTPUT)/ell_omp: bin/ell/ell_omp.cpp $(DEPS)
	$(CXX) -Ibin/ell $(BASE) bin/ell/ell_omp.cpp -o $(BIN_OUTPUT)/ell_omp $(CXXFLAGS) $(OMPFLAGS)

$(BIN_OUTPUT)/ell_omp_gpu: bin/ell/ell_omp_gpu.cpp $(DEPS)
	$(CXX) -Ibin/ell $(BASE) bin/ell/ell_omp_gpu.cpp -o $(BIN_OUTPUT)/ell_omp_gpu $(CXXFLAGS) $(OMPFLAGS) $(GPUFLAGS)
	
##
## Builds the BCSR executables
##
$(BIN_OUTPUT)/bcsr_serial: bin/bcsr/bcsr_serial.cpp $(DEPS)
	$(CXX) -Ibin/bcsr $(BASE) bin/bcsr/bcsr_serial.cpp -o $(BIN_OUTPUT)/bcsr_serial $(CXXFLAGS)

$(BIN_OUTPUT)/bcsr_omp: bin/bcsr/bcsr_omp.cpp $(DEPS)
	$(CXX)  -Ibin/bcsr $(BASE) bin/bcsr/bcsr_omp.cpp -o $(BIN_OUTPUT)/bcsr_omp $(CXXFLAGS) $(OMPFLAGS)

$(BIN_OUTPUT)/bcsr_omp_gpu: bin/bcsr/bcsr_omp_gpu.cpp $(DEPS)
	$(CXX)  -Ibin/bcsr $(BASE) bin/bcsr/bcsr_omp_gpu.cpp -o $(BIN_OUTPUT)/bcsr_omp_gpu $(CXXFLAGS) $(OMPFLAGS) $(GPUFLAGS)

########################################################################

##
## Clean target
##
.PHONY: clean
clean:
	rm -rf build/*


