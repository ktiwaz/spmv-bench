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

## COO- All types
kernel = [ "serial", "omp", "gpu" ]
frame = create_data(matrix, ["coo"], kernel, ["arm"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "COO- All Types", "MFLOPS", "Matrix", "Name", output= "study2_coo_arm")

## CSR- All types
kernel = [ "serial", "omp", "gpu" ]
frame = create_data(matrix, ["csr"], kernel, ["arm"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "CSR- All Types", "MFLOPS", "Matrix", "Name", output= "study2_csr_arm")

## ELL- All types
kernel = [ "serial", "omp", "gpu" ]
frame = create_data(matrix, ["ell"], kernel, ["arm"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "ELL- All Types", "MFLOPS", "Matrix", "Name", output= "study2_ell_arm")

