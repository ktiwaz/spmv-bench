from plot import *

matrix = [
    #"2cubes_sphere",
    "af23560",
    "bcsstk13",
    "bcsstk17",
    "cant",
    #"cop20k_A",
    "crankseg_2",
    "dw4096",
    "nd24k",
    "pdb1HYS",
    "rma10",
    #"shallow_water1",
    #"torso1",
    #"x104",
]

## COO- All types
kernel = [ "cusparse", "gpu_full" ]
frame = create_data(matrix, ["coo"], kernel, ["arm"], filter_eq=[])
plot_grouped_bar(frame, "COO- cuSparse vs GPU", "MFLOPS", "Matrix", "Name", output= "study7_coo_arm")

## CSR- All types
kernel = [ "cusparse", "gpu_full" ]
frame = create_data(matrix, ["csr"], kernel, ["arm"], filter_eq=[])
plot_grouped_bar(frame, "CSR- cuSparse vs GPU", "MFLOPS", "Matrix", "Name", output= "study7_csr_arm")


