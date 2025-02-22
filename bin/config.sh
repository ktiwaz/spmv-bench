BIN=build/bin


# For benchmarking (Old)
#OLEVELS=("O1" "O2" "O3")
#iters=100
#threads=(8 16 32 64)
#blocks=(1 2 4 16)
#k_loop=(16)

# For benchmarking (Shorter- hopefully)
OLEVELS=("O3")
iters=10
#threads=(8 16 32)
#blocks=(2 4 16)
#k_loop=(16 64 128)
#blocks=(2 4 16)

#iters=1
threads=(8)
blocks=(4)
# k_loop=(8 16 64 128 256 512 1028)

iters_tlist=5
tlist=2,4,8,16,32,48,64,72

# For testing
#OLEVELS=("O1" "O2" "O3")
#iters=1
#threads=(8 16)
#blocks=(1 4)
#k_loop=(16)

