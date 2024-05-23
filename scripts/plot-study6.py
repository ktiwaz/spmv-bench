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

# COO- Architecture study
frame = create_data(matrix, ["coo"], ["serial"], ["arm", "intel"], filter_eq=[("K-Bound", 128)])
frame = change_names(frame, "Archs", " ", True)
plot_grouped_bar(frame, "x86 vs Arm- COO", "MFLOPS", "Matrix", "Name", output= "study6_coo_cpu")

# CSR- Architecture study
frame = create_data(matrix, ["csr"], ["serial"], ["arm", "intel"], filter_eq=[("K-Bound", 128)])
frame = change_names(frame, "Archs", " ", True)
plot_grouped_bar(frame, "x86 vs Arm- CSR", "MFLOPS", "Matrix", "Name", output= "study6_csr_cpu")

# ELL- Architecture study
frame = create_data(matrix, ["ell"], ["serial"], ["arm", "intel"], filter_eq=[("K-Bound", 128)])
frame = change_names(frame, "Archs", " ", True)
plot_grouped_bar(frame, "x86 vs Arm- ELL", "MFLOPS", "Matrix", "Name", output= "study6_ell_cpu")

# BCSR- Architecture study
frame = create_data(matrix, ["bcsr"], ["serial"], ["arm", "intel"], filter_eq=[("K-Bound", 128), ("Block Row", 4), ("Block Col", 4)])
frame = change_names(frame, "Archs", " ", True)
plot_grouped_bar(frame, "x86 vs Arm- BCSR (4x4)", "MFLOPS", "Matrix", "Name", output= "study6_bcsr_cpu")

# BCSR- Architecture study 2
frame = create_data(matrix, ["bcsr"], ["serial"], ["arm", "intel"], filter_eq=[("K-Bound", 128), ("Block Row", 2), ("Block Col", 2)])
frame = change_names(frame, "Archs", " ", True)
plot_grouped_bar(frame, "x86 vs Arm- BCSR (2x2)", "MFLOPS", "Matrix", "Name", output= "study6_bcsr2_cpu")

frame = create_data(matrix, ["bcsr"], ["serial"], ["arm", "intel"], filter_eq=[("K-Bound", 128), ("Block Row", 16), ("Block Col", 16)])
frame = change_names(frame, "Archs", " ", True)
plot_grouped_bar(frame, "x86 vs Arm- BCSR (16x16)", "MFLOPS", "Matrix", "Name", output= "study6_bcsr16_cpu")

