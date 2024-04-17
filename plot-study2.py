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

########################################################
## Arm
########################################################

## COO- All types
kernel = [ "serial", "omp", "gpu" ]
frame = create_data(matrix, ["coo"], kernel, ["arm"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "COO- All Types (Arm)", "MFLOPS", "Matrix", "Name", output= "study2_coo_arm")

## CSR- All types
kernel = [ "serial", "omp", "gpu" ]
frame = create_data(matrix, ["csr"], kernel, ["arm"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "CSR- All Types (Arm)", "MFLOPS", "Matrix", "Name", output= "study2_csr_arm")

## ELL- All types
kernel = [ "serial", "omp", "gpu" ]
frame = create_data(matrix, ["ell"], kernel, ["arm"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "ELL- All Types (Arm)", "MFLOPS", "Matrix", "Name", output= "study2_ell_arm")

## BCSR- All types
kernel = [ "serial", "omp", "gpu" ]
frame = create_data(matrix, ["bcsr"], kernel, ["arm"], filter_eq=[("K-Bound", 128), ("Threads", 32), ("Block Row", 4), ("Block Col", 4)])
plot_grouped_bar(frame, "BCSR- All Types (Arm)", "MFLOPS", "Matrix", "Name", output= "study2_bcsr_arm")


########################################################
## Intel (with GPU)
########################################################

## COO- All types
kernel = [ "serial", "omp"]
frame = create_data(matrix, ["coo"], kernel, ["intel"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "COO- All Types (Intel)", "MFLOPS", "Matrix", "Name", output= "study2_coo_intel")

## CSR- All types
kernel = [ "serial", "omp"]
frame = create_data(matrix, ["csr"], kernel, ["intel"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "CSR- All Types (Intel)", "MFLOPS", "Matrix", "Name", output= "study2_csr_intel")

## ELL- All types
kernel = [ "serial", "omp"]
frame = create_data(matrix, ["ell"], kernel, ["intel"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "ELL- All Types (Intel)", "MFLOPS", "Matrix", "Name", output= "study2_ell_intel")

## BCSR- All types
kernel = [ "serial", "omp"]
frame = create_data(matrix, ["bcsr"], kernel, ["intel"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "BCSR- All Types (Intel)", "MFLOPS", "Matrix", "Name", output= "study2_bcsr_intel")


########################################################
## Intel (with GPU)
########################################################

matrix = [
    #"2cubes_sphere",
    "af23560",
    "bcsstk13",
    "bcsstk17",
    #"cant",
    #"cop20k_A",
    #"crankseg_2",
    "dw4096",
    #"nd24k",
    "pdb1HYS",
    "rma10",
    #"shallow_water1",
    #"torso1",
    #"x104",
]

## COO- All types
kernel = [ "serial", "omp", "gpu" ]
frame = create_data(matrix, ["coo"], kernel, ["intel"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "COO- All Types (Intel)", "MFLOPS", "Matrix", "Name", output= "study2_coo_intel_gpu")

## CSR- All types
kernel = [ "serial", "omp", "gpu" ]
frame = create_data(matrix, ["csr"], kernel, ["intel"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "CSR- All Types (Intel)", "MFLOPS", "Matrix", "Name", output= "study2_csr_intel_gpu")

## ELL- All types
kernel = [ "serial", "omp", "gpu" ]
frame = create_data(matrix, ["ell"], kernel, ["intel"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "ELL- All Types (Intel)", "MFLOPS", "Matrix", "Name", output= "study2_ell_intel_gpu")

## BCSR- All types
kernel = [ "serial", "omp", "gpu" ]
frame = create_data(matrix, ["bcsr"], kernel, ["intel"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "BCSR- All Types (Intel)", "MFLOPS", "Matrix", "Name", output= "study2_bcsr_intel_gpu")

