from plot import *

##
## If we want to create a serial plot for each format across all benchmarks
##
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

fmt = [ "coo", "csr" ]

##
## Plot 1: show the serial output of all formats across all matrices on Arm
##
kernel = [ "serial" ]
frame = create_data(matrix, fmt, kernel, ["arm"], filter_eq=[("K-Bound", 128)])
plot_grouped_bar(frame, "Serial Formats (Arm)", "MFLOPS", "Matrix", "Name", output= "all_format_serial_arm")

##
## Plot 2: show the parallel output of all formats across all matrices on Arm
##
kernel = [ "omp" ]
frame = create_data(matrix, fmt, kernel, ["arm"], filter_eq=[("K-Bound", 128), ("Threads", 8)])
plot_grouped_bar(frame, "Parallel Formats (Arm)", "MFLOPS", "Matrix", "Name", output= "all_format_omp_arm")

##
## Plot 3: show the parallel output of all formats across all matrices on Arm
##
kernel = [ "gpu" ]
frame = create_data(matrix, fmt, kernel, ["arm"], filter_eq=[("K-Bound", 128)])
plot_grouped_bar(frame, "GPU Formats (Arm)", "MFLOPS", "Matrix", "Name", output= "all_format_gpu_arm")

##
## Plot 4: show the COO output for all kernels across all matrices on Arm
##
kernel = [ "serial", "omp", "gpu" ]
frame = create_data(matrix, ["coo"], kernel, ["arm"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "COO- All Types", "MFLOPS", "Matrix", "Name", output= "all_coo_arm")

##
## Plot 5: show the COO output for all parallel kernels across all matrices on Arm
##
frame = create_data(matrix, ["coo"], ["omp"], ["arm"], filter_eq=[("K-Bound", 128)])
frame = change_names(frame, "Threads", "-t")
plot_grouped_bar(frame, "COO- Parallel (Arm)", "MFLOPS", "Matrix", "Name", output= "all_coo_parallel_arm")

##
## Plot 6: show the CSR output for all kernels across all matrices on Arm
##
kernel = [ "serial", "omp", "gpu" ]
frame = create_data(matrix, ["csr"], kernel, ["arm"], filter_eq=[("K-Bound", 128), ("Threads", 32)])
plot_grouped_bar(frame, "CSR- All Types (Arm)", "MFLOPS", "Matrix", "Name", output= "all_csr_arm")

##
## Plot 7: show the CSR output for all parallel kernels across all matrices on Arm
##
frame = create_data(matrix, ["csr"], ["omp"], ["arm"], filter_eq=[("K-Bound", 128)])
frame = change_names(frame, "Threads", " -t ", True)
plot_grouped_bar(frame, "CSR- Parallel (Arm)", "MFLOPS", "Matrix", "Name", output= "all_csr_parallel_arm")

##
## Plot 8: show the COO output for the k-study across all matrices on Arm
##
frame = create_data(matrix, ["coo"], ["serial", "omp"], ["arm"])
frame = change_names(frame, "K-Bound", " -k ")
plot_grouped_bar(frame, "COO- Serial- K Study (Arm)", "MFLOPS", "Matrix", "Name", output= "all_coo_k_arm")

##
## Plot 9: show the CSR output for the k-study across all matrices on Arm
##
frame = create_data(matrix, ["csr"], ["serial", "omp"], ["arm"])
frame = change_names(frame, "K-Bound", " -k ")
plot_grouped_bar(frame, "CSR- Serial- K Study (Arm)", "MFLOPS", "Matrix", "Name", output= "all_csr_k_arm")

