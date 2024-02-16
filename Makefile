CC=clang
CXX=clang++
CXX16=/opt/llvm/llvm-16.x-install/bin/clang++
CXXFLAGS=-Isrc build/libspmm.a -std=c++17 -O2 -march=native
OMPFLAGS=-fopenmp
GPUFLAGS=

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

build/spmm.o: src/spmm.cpp src/spmm.h src/csr/csr.h src/ell/ell.h src/bell/bell.h src/bcsr/bcsr.h
	$(CXX) src/spmm.cpp -c -o build/spmm.o -O2 -fPIE -march=native
	
##
## Build the test binaries
##
$(TEST_OUTPUT)/%: test/%.cpp build/libspmm.a
	$(CXX) $< -o $@ $(CXXFLAGS) $(OMPFLAGS)

$(GPU_TEST_OUTPUT)/%: test/gpu/%.cpp build/libspmm.a
	$(CXX) $< -o $@ $(CXXFLAGS) $(OMPFLAGS) $(GPUFLAGS)
	
########################################################################
BASE=src/main.cpp build/libspmm.a
DEPS=build/libspmm.a src/main.cpp

##
## Builds the COO executables
##
$(BIN_OUTPUT)/coo_serial: src/coo/coo_serial.cpp $(DEPS)
	$(CXX) -Isrc/coo $(BASE) src/coo/coo_serial.cpp -o $(BIN_OUTPUT)/coo_serial $(CXXFLAGS)

$(BIN_OUTPUT)/coo_omp: src/coo/coo_omp.cpp $(DEPS)
	$(CXX) -Isrc/coo $(BASE) src/coo/coo_omp.cpp -o $(BIN_OUTPUT)/coo_omp $(CXXFLAGS) $(OMPFLAGS)
	
$(BIN_OUTPUT)/coo_omp_gpu: src/coo/coo_omp_gpu.cpp $(DEPS)
	$(CXX) -Isrc/coo $(BASE) src/coo/coo_omp_gpu.cpp -o $(BIN_OUTPUT)/coo_omp_gpu $(CXXFLAGS) $(OMPFLAGS) $(GPUFLAGS)

##
## Builds the CSR executables
##
$(BIN_OUTPUT)/csr_serial: src/csr/csr_serial.cpp $(DEPS)
	$(CXX) -Isrc/csr $(BASE) src/csr/csr_serial.cpp -o $(BIN_OUTPUT)/csr_serial $(CXXFLAGS)

$(BIN_OUTPUT)/csr_omp: src/csr/csr_omp.cpp $(DEPS)
	$(CXX) -Isrc/csr $(BASE) src/csr/csr_omp.cpp -o $(BIN_OUTPUT)/csr_omp $(CXXFLAGS) $(OMPFLAGS)

$(BIN_OUTPUT)/csr_omp_gpu: src/csr/csr_omp_gpu.cpp $(DEPS)
	$(CXX) -Isrc/csr $(BASE) src/csr/csr_omp_gpu.cpp -o $(BIN_OUTPUT)/csr_omp_gpu $(CXXFLAGS) $(OMPFLAGS) $(GPUFLAGS)

##
## Builds the ELL executables
##
$(BIN_OUTPUT)/ell_serial: src/ell/ell_serial.cpp $(DEPS)
	$(CXX) -Isrc/ell $(BASE) src/ell/ell_serial.cpp -o $(BIN_OUTPUT)/ell_serial $(CXXFLAGS)

$(BIN_OUTPUT)/ell_omp: src/ell/ell_omp.cpp $(DEPS)
	$(CXX) -Isrc/ell $(BASE) src/ell/ell_omp.cpp -o $(BIN_OUTPUT)/ell_omp $(CXXFLAGS) $(OMPFLAGS)

$(BIN_OUTPUT)/ell_omp_gpu: src/ell/ell_omp_gpu.cpp $(DEPS)
	$(CXX) -Isrc/ell $(BASE) src/ell/ell_omp_gpu.cpp -o $(BIN_OUTPUT)/ell_omp_gpu $(CXXFLAGS) $(OMPFLAGS) $(GPUFLAGS)
	
##
## Builds the BCSR executables
##
$(BIN_OUTPUT)/bcsr_serial: src/bcsr/bcsr_serial.cpp $(DEPS)
	$(CXX) -Isrc/bcsr $(BASE) src/bcsr/bcsr_serial.cpp -o $(BIN_OUTPUT)/bcsr_serial $(CXXFLAGS)

$(BIN_OUTPUT)/bcsr_omp: src/bcsr/bcsr_omp.cpp $(DEPS)
	$(CXX)  -Isrc/bcsr $(BASE) src/bcsr/bcsr_omp.cpp -o $(BIN_OUTPUT)/bcsr_omp $(CXXFLAGS) $(OMPFLAGS)

$(BIN_OUTPUT)/bcsr_omp_gpu: src/bcsr/bcsr_omp_gpu.cpp $(DEPS)
	$(CXX)  -Isrc/bcsr $(BASE) src/bcsr/bcsr_omp_gpu.cpp -o $(BIN_OUTPUT)/bcsr_omp_gpu $(CXXFLAGS) $(OMPFLAGS) $(GPUFLAGS)

########################################################################

##
## Clean target
##
.PHONY: clean
clean:
	rm -rf build/*


