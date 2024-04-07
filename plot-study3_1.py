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

fmt = [ "coo", "csr", "ell", "bcsr" ]
#fmt = [ "coo" ]

########################################################
## Arm
########################################################
frame = create_data(matrix, fmt, ["omp_threads"], ["arm"], filter_eq=[("K-Bound", 128), ("Block Row", 4), ("Block Col", 4)])
frame = change_names(frame, "", "", True, True)
plot_grouped_bar(frame, "Parallel- Best Thread (Arm)", "Threads", "Matrix", "Name", output= "study3_1_omp_threads_arm")

########################################################
## Intel
########################################################
frame = create_data(matrix, fmt, ["omp_threads"], ["intel"], filter_eq=[("K-Bound", 128), ("Block Row", 4), ("Block Col", 4)])
frame = change_names(frame, "", "", True, True)
plot_grouped_bar(frame, "Parallel- Best Thread (Intel)", "Threads", "Matrix", "Name", output= "study3_1_omp_threads_intel")


