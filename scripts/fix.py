import pandas as pd

old = "/home/patrick/Downloads/spmm-bench-old/csv/"
new = "./report/csv/"

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

#m = matrix[0]
for m in matrix:
    # OMP Intel
    omp_f_intel1 = old + m + "_bcsr_omp_intel.csv"
    omp_f_intel2 = new + m + "_bcsr_omp_intel.csv"

    df = pd.concat(map(pd.read_csv, [omp_f_intel1, omp_f_intel2]), ignore_index=True) 
    print(df)
    df.to_csv(omp_f_intel2)

    # OMP Arm
    omp_f_arm1 = old + m + "_bcsr_omp_arm.csv"
    omp_f_arm2 = new + m + "_bcsr_omp_arm.csv"

    df = pd.concat(map(pd.read_csv, [omp_f_arm1, omp_f_arm2]), ignore_index=True) 
    print(df)
    df.to_csv(omp_f_arm2)

    # Serial Intel
    serial_f_intel1 = old + m + "_bcsr_serial_intel.csv"
    serial_f_intel2 = new + m + "_bcsr_serial_intel.csv"

    df = pd.concat(map(pd.read_csv, [serial_f_intel1, serial_f_intel2]), ignore_index=True) 
    print(df)
    df.to_csv(serial_f_intel2)

    # Serial Arm
    serial_f_arm1 = old + m + "_bcsr_serial_arm.csv"
    serial_f_arm2 = new + m + "_bcsr_serial_arm.csv"

    df = pd.concat(map(pd.read_csv, [serial_f_arm1, serial_f_arm2]), ignore_index=True) 
    print(df)
    df.to_csv(serial_f_arm2)

