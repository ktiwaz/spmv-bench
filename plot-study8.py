from plot import *

matrix = [
    "2cubes_sphere",
    "af23560",
    "bcsstk13",
    "bcsstk17",
    "cant",
    "cop20k_A",
    "crankseg_2",
    "dw4096",
    "nd24k",
    "pdb1HYS",
    "rma10",
    "shallow_water1",
    "torso1",
    "x104",
]

###############################################################
## Arm
###############################################################

## COO- OMP
kernel = [ "omp", "omp_transpose" ]
frame = create_data(matrix, ["coo"], kernel, ["arm"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "COO- OMP vs OMP Transpose (Arm)", "MFLOPS", "Matrix", "Name", output= "study8_coo_omp_arm")

## CSR- OMP
kernel = [ "omp", "omp_transpose" ]
frame = create_data(matrix, ["csr"], kernel, ["arm"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "CSR- OMP vs OMP Transpose (Arm)", "MFLOPS", "Matrix", "Name", output= "study8_csr_omp_arm")

## ELL- OMP
kernel = [ "omp", "omp_transpose" ]
frame = create_data(matrix, ["ell"], kernel, ["arm"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "ELL- OMP vs OMP Transpose (Arm)", "MFLOPS", "Matrix", "Name", output= "study8_ell_omp_arm")

## BCSR- OMP
kernel = [ "omp", "omp_transpose" ]
frame = create_data(matrix, ["bcsr"], kernel, ["arm"], filter_eq=[("K-Bound", 128), ("Block Row", 4), ("Block Col", 4), ("Threads", 32)])
plot_grouped_bar(frame, "BCSR- OMP vs OMP Transpose (Arm)", "MFLOPS", "Matrix", "Name", output= "study8_bcsr_omp_arm")


###############################################################
## Intel
###############################################################

## COO- OMP
kernel = [ "omp", "omp_transpose" ]
frame = create_data(matrix, ["coo"], kernel, ["intel"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "COO- OMP vs OMP Transpose (Intel)", "MFLOPS", "Matrix", "Name", output= "study8_coo_omp_intel")

## CSR- OMP
kernel = [ "omp", "omp_transpose" ]
frame = create_data(matrix, ["csr"], kernel, ["intel"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "CSR- OMP vs OMP Transpose (Intel)", "MFLOPS", "Matrix", "Name", output= "study8_csr_omp_intel")

## ELL- OMP
kernel = [ "omp", "omp_transpose" ]
frame = create_data(matrix, ["ell"], kernel, ["intel"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "ELL- OMP vs OMP Transpose (Intel)", "MFLOPS", "Matrix", "Name", output= "study8_ell_omp_intel")

## BCSR- OMP
kernel = [ "omp", "omp_transpose" ]
frame = create_data(matrix, ["bcsr"], kernel, ["intel"], filter_eq=[("K-Bound", 128), ("Block Row", 4), ("Block Col", 4), ("Threads", 32)])
plot_grouped_bar(frame, "BCSR- OMP vs OMP Transpose (Intel)", "MFLOPS", "Matrix", "Name", output= "study8_bcsr_omp_intel")

