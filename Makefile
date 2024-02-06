CC=clang
CXX=clang++
CXXFLAGS=-Ilib build/libspmm.a -std=c++17
	
TEST_SRC := $(wildcard test/*.cpp)
TEST_OUTPUT = build/test
TEST_BINS := $(patsubst test/%.cpp,$(TEST_OUTPUT)/%,$(TEST_SRC))

all: check_dir build/libspmm.a $(TEST_BINS)

.PHONY: clean_dir
check_dir:
	if [ ! -d ./build ]; then mkdir build; fi; \
	if [ ! -d ./build/test ] ; then mkdir build/test; fi;

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

##
## Clean target
##
.PHONY: clean
clean:
	rm -rf build/*

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

