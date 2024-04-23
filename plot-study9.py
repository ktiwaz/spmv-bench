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


########################################################
## Arm
########################################################

frame = create_data(matrix, fmt, ["serial"], ["arm", "arm2"], filter_eq=[("K-Bound", 128), ("Block Row", 4), ("Block Col", 4)])
frame = change_names(frame, "Archs", " ", True)
plot_grouped_bar(frame, "Serial- All Types (Arm)", "MFLOPS", "Matrix", "Name", output= "study9_serial_arm")

frame = create_data(matrix, fmt, ["omp"], ["arm", "arm2"], filter_eq=[("K-Bound", 128), ("Block Row", 4), ("Block Col", 4), ("Threads", 32)])
frame = change_names(frame, "Archs", " ", True)
plot_grouped_bar(frame, "OMP- All Types (Arm)", "MFLOPS", "Matrix", "Name", output= "study9_omp_arm")


########################################################
## x86
########################################################

frame = create_data(matrix, fmt, ["serial"], ["intel", "intel2"], filter_eq=[("K-Bound", 128), ("Block Row", 4), ("Block Col", 4)])
frame = change_names(frame, "Archs", " ", True)
plot_grouped_bar(frame, "Serial- All Types (x86)", "MFLOPS", "Matrix", "Name", output= "study9_serial_intel")

frame = create_data(matrix, fmt, ["omp"], ["intel", "intel2"], filter_eq=[("K-Bound", 128), ("Block Row", 4), ("Block Col", 4), ("Threads", 32)])
frame = change_names(frame, "Archs", " ", True)
plot_grouped_bar(frame, "OMP- All Types (x86)", "MFLOPS", "Matrix", "Name", output= "study9_omp_intel")

