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

## Study 3- COO
frame = create_data(matrix, ["coo"], ["omp"], ["arm"], filter_eq=[("K-Bound", 128)])
frame = change_names(frame, "Threads", " -t ", True)
plot_grouped_bar(frame, "COO- Parallel (Arm)", "MFLOPS", "Matrix", "Name", output= "study3_coo_arm")

## Study 3- CSR
frame = create_data(matrix, ["csr"], ["omp"], ["arm"], filter_eq=[("K-Bound", 128)])
frame = change_names(frame, "Threads", " -t ", True)
plot_grouped_bar(frame, "CSR- Parallel (Arm)", "MFLOPS", "Matrix", "Name", output= "study3_csr_arm")

## Study 3- ELL
frame = create_data(matrix, ["ell"], ["omp"], ["arm"], filter_eq=[("K-Bound", 128)])
frame = change_names(frame, "Threads", " -t ", True)
plot_grouped_bar(frame, "ELL- Parallel (Arm)", "MFLOPS", "Matrix", "Name", output= "study3_ell_arm")

## Study 3- BCSR
frame = create_data(matrix, ["bcsr"], ["omp"], ["arm"], filter_eq=[("K-Bound", 128), ("Block Row", 4), ("Block Col", 4)])
frame = change_names(frame, "Threads", " -t ", True)
plot_grouped_bar(frame, "BCSR- Parallel (Arm)", "MFLOPS", "Matrix", "Name", output= "study3_bcsr_arm")

