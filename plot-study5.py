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

## Study 5- Serial
frame = create_data(matrix, ["bcsr"], ["serial"], ["arm"], filter_eq=[("K-Bound", 128)])
frame = change_names(frame, "Block Row", " -b ", True)
plot_grouped_bar(frame, "BCSR- Serial (Arm)", "MFLOPS", "Matrix", "Name", output= "study5_serial_arm")

## Study 5- Parallel
frame = create_data(matrix, ["bcsr"], ["omp"], ["arm"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
frame = change_names(frame, "Block Row", " -b ", True)
plot_grouped_bar(frame, "BCSR- Parallel (Arm)", "MFLOPS", "Matrix", "Name", output= "study5_parallel_arm")

## Study 5- GPU
frame = create_data(matrix, ["bcsr"], ["gpu"], ["arm"], filter_eq=[("K-Bound", 128)])
frame = change_names(frame, "Block Row", " -b ", True)
plot_grouped_bar(frame, "BCSR- GPU (Arm)", "MFLOPS", "Matrix", "Name", output= "study5_gpu_arm")

