## All benchmarks have this format:
## <matrix>_<format>_<type>_<architecture>.csv
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

csv_path = "report/csv/"
img_path = "report/images/"

def test_plot():
    df = pd.DataFrame([['g1','c1',10],['g1','c2',12],['g1','c3',13],['g2','c1',8],
                       ['g2','c2',10],['g2','c3',12]],columns=['group','column','val'])

    df.pivot("column", "group", "val").plot(kind='bar')

    plt.show()

def load_raw_data(bench_item):
    df = pd.read_csv(bench_item[1])
    m_col = []
    for i in range(df.shape[0]): m_col.append(bench_item[0])
    df["Matrix"] = m_col
    print(df)
    return df

def create_data(matrix, fmt, kernel, arch, filter_eq=[]):
    # Create a list of all files that we need to read
    benchmarks = []
    for m in matrix:
        for f in fmt:
            for k in kernel:
                for a in arch:
                    name = m + "_" + f + "_" + k + "_" + a + ".csv"
                    benchmarks.append((m, csv_path + name))
    
    # Create a CSV plot
    df = pd.concat(map(load_raw_data, benchmarks))
    print(df)
    
    #df = df[["Name", "Matrix", "MFLOPS", "K-Bound"]]
    #print(df)
    for f in filter_eq:
        print(df[f[0]])
        df = df[(df[f[0]] == f[1]) | (df[f[0]] == -1)]
    #print(df)
    #df.to_csv("filter.csv")
    return df

def change_names(df, key, prefix, rename_keys = False):
    if rename_keys:
        df["Name"] = df.apply(lambda row: row["Name"].split()[0], axis=1)
    df["Name"] = df.apply(lambda row: row["Name"] + str(prefix) + str(row[key]), axis=1)
    return df
    
def plot_grouped_bar(df, title, values, index, columns, output=None):
    df_pivot = pd.pivot_table(
	    df,
	    values="MFLOPS",
	    index="Matrix",
	    columns="Name",
	    #aggfunc=np.mean
    )
    
    plot = df_pivot.plot(kind="bar")
    plot.tick_params(axis='x', labelrotation=70)
    plot.set_title(title)
    plot.set_xlabel(index)
    plot.set_ylabel(values)
    if output is None:
        plt.show()
    else:
        fig = plot.get_figure()
        fig.savefig(img_path + output + ".pdf", bbox_inches="tight")

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

