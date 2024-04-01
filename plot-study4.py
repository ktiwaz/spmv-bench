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

# COO
frame = create_data(matrix, ["coo"], ["serial", "omp"], ["arm"])
frame = change_names(frame, "K-Bound", " -k ")
plot_grouped_bar(frame, "COO- Serial- K Study (Arm)", "MFLOPS", "Matrix", "Name", output= "study4_coo_arm")

# CSR
frame = create_data(matrix, ["csr"], ["serial", "omp"], ["arm"])
frame = change_names(frame, "K-Bound", " -k ")
plot_grouped_bar(frame, "CSR- Serial- K Study (Arm)", "MFLOPS", "Matrix", "Name", output= "study4_csr_arm")

# ELL
frame = create_data(matrix, ["ell"], ["serial", "omp"], ["arm"])
frame = change_names(frame, "K-Bound", " -k ")
plot_grouped_bar(frame, "ELL- Serial- K Study (Arm)", "MFLOPS", "Matrix", "Name", output= "study4_ell_arm")

