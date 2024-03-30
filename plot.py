## All benchmarks have this format:
## <matrix>_<format>_<type>_<architecture>.csv
import pandas as pd
import matplotlib.pyplot as plt

csv_path = "report/csv/"

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

def create_data(matrix, fmt, kernel, arch):
    # Create a list of all files that we need to read
    benchmarks = []
    for m in matrix:
        for f in fmt:
            for k in kernel:
                name = m + "_" + f + "_" + k + "_" + arch + ".csv"
                benchmarks.append((m, csv_path + name))
    
    #print(benchmarks)
    
    # Create a CSV plot
    df = pd.concat(map(load_raw_data, benchmarks))
    print(df)
    
    df = df[["Name", "Matrix", "MFLOPS", "K-Bound"]]
    print(df)
    df = df[df["K-Bound"] == 128]
    print(df)
    
    df_pivot = pd.pivot_table(
	    df,
	    values="MFLOPS",
	    index="Matrix",
	    columns="Name",
    )
    
    df_pivot.plot(kind="bar")
    plt.show()

##
## If we want to create a serial plot for each format across all benchmarks
##
matrix = [
    "2cubes_sphere",
    "bcsstk17",
    "cant",
    "cop20k_A",
]

fmt = [ "coo", "csr" ]
kernel = [ "serial" ]
arch = "arm"

create_data(matrix, fmt, kernel, arch)
