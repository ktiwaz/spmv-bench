#!/usr/bin/python3
import random
import sys

rows = int(sys.argv[1])
cols = int(sys.argv[2])
dense_args = int(sys.argv[3])
if dense_args == cols:
    make_dense = True
else:
    vals = dense_args
    make_dense = False

#rows = 1024
#cols = 1024
#make_dense = False

out_cols = 0
#vals = int(cols/128)

data = list()

# Makes a fully dense matrix
if make_dense:
    vals = rows * cols
    out_cols = cols
    for r in range(rows):
        for c in range(cols):
            v = float(random.uniform(0.1, 10.0))
            s = '{:.1f}'.format(v)
            line = str(r + 1) + " " + str(c + 1) + " " + str(s)
            data.append(line)

# Make a sparse ellpack matrix with a defined number of columns
else:
    ell_cols = vals
    out_cols = ell_cols
    vals = rows * ell_cols
    for r in range(rows):
        local_cols = list()
        for c in range(ell_cols):
            col = random.randint(1, cols)
            local_cols.append(col)
        local_cols.sort()
        for col in local_cols:
            v = float(random.uniform(0.1, 10.0))
            s = '{:.1f}'.format(v)
            line = str(r + 1) + " " + str(col) + " " + str(s)
            data.append(line)

writer = open(str(rows) + "x" + str(out_cols) + ".mtx", "w")
writer.write("%%MatrixMarket matrix coordinate real general\n")
writer.write("%\n")
writer.write("%\n")
writer.write(str(rows) + " " + str(cols) + " " + str(vals) + "\n")
for ln in data:
    writer.write(ln)
    writer.write("\n")
writer.close()

