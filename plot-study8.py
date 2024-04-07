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

## COO- Serial
kernel = [ "serial", "serial_transpose" ]
frame = create_data(matrix, ["coo"], kernel, ["arm"], filter_eq=[("K-Bound", 128)])
plot_grouped_bar(frame, "COO- Serial vs Serial Transpose (Arm)", "MFLOPS", "Matrix", "Name", output= "study8_coo_serial_arm")

## CSR- Serial
kernel = [ "serial", "serial_transpose" ]
frame = create_data(matrix, ["csr"], kernel, ["arm"], filter_eq=[("K-Bound", 128)])
plot_grouped_bar(frame, "CSR- Serial vs Serial Transpose (Arm)", "MFLOPS", "Matrix", "Name", output= "study8_csr_serial_arm")

## ELL- Serial
kernel = [ "serial", "serial_transpose" ]
frame = create_data(matrix, ["ell"], kernel, ["arm"], filter_eq=[("K-Bound", 128)])
plot_grouped_bar(frame, "ELL- Serial vs Serial Transpose (Arm)", "MFLOPS", "Matrix", "Name", output= "study8_ell_serial_arm")

## BCSR- Serial
#kernel = [ "serial", "serial_transpose" ]
#frame = create_data(matrix, ["bcsr"], kernel, ["arm"], filter_eq=[("K-Bound", 128), ("Block Row", 4), ("Block Col", 4)])
#plot_grouped_bar(frame, "BCSR- Serial vs Serial Transpose (Arm)", "MFLOPS", "Matrix", "Name", output= "study8_bcsr_serial_arm")

###############################################################
## Intel
###############################################################

## COO- Serial
kernel = [ "serial", "serial_transpose" ]
frame = create_data(matrix, ["coo"], kernel, ["intel"], filter_eq=[("K-Bound", 128)])
plot_grouped_bar(frame, "COO- Serial vs Serial Transpose (Intel)", "MFLOPS", "Matrix", "Name", output= "study8_coo_serial_intel")

## CSR- Serial
kernel = [ "serial", "serial_transpose" ]
frame = create_data(matrix, ["csr"], kernel, ["intel"], filter_eq=[("K-Bound", 128)])
plot_grouped_bar(frame, "CSR- Serial vs Serial Transpose (Intel)", "MFLOPS", "Matrix", "Name", output= "study8_csr_serial_intel")

## ELL- Serial
kernel = [ "serial", "serial_transpose" ]
frame = create_data(matrix, ["ell"], kernel, ["intel"], filter_eq=[("K-Bound", 128)])
plot_grouped_bar(frame, "ELL- Serial vs Serial Transpose (Intel)", "MFLOPS", "Matrix", "Name", output= "study8_ell_serial_intel")

## BCSR- Serial
#kernel = [ "serial", "serial_transpose" ]
#frame = create_data(matrix, ["bcsr"], kernel, ["intel"], filter_eq=[("K-Bound", 128), ("Block Row", 4), ("Block Col", 4)])
#plot_grouped_bar(frame, "BCSR- Serial vs Serial Transpose (Intel)", "MFLOPS", "Matrix", "Name", output= "study8_bcsr_serial_intel")

