#!/usr/bin/python
import csv
import sys
import os

input_file = str(sys.argv[1])
csv_file = "report/csv/" + input_file + ".csv"
output_path = "report/csv/" + input_file + "/"
os.makedirs(output_path, exist_ok = True)

ell_all = list()
ell_serial = list()
ell_omp = list()

bcsr_all = list()
bcsr_serial = list()
bcsr_omp = list()

with open(csv_file) as f:
    reader = csv.reader(f)
    header = False
    for row in reader:
        if not header:
            ell_all.append(row)
            ell_serial.append(row)
            ell_omp.append(row)
            bcsr_all.append(row)
            bcsr_serial.append(row)
            bcsr_omp.append(row)
            header = True
            continue
        
        key = row[0]
        if key.startswith("csr"):
            ell_all.append(row)
            bcsr_all.append(row)
        if key.startswith("csr_serial"):
            ell_serial.append(row)
            bcsr_serial.append(row)
        if key.startswith("csr_omp"):
            ell_omp.append(row)
            bcsr_omp.append(row)
        if key.startswith("ell"):
            ell_all.append(row)
        if key.startswith("ell_serial"):
            ell_serial.append(row)
        if key.startswith("ell_omp"):
            ell_omp.append(row)
        if key.startswith("bcsr"):
            bcsr_all.append(row)
        if key.startswith("bcsr_serial"):
            bcsr_serial.append(row)
        if key.startswith("bcsr_omp"):
            bcsr_omp.append(row)
            
all_data = {
    "ell_all": ell_all,
    "ell_serial": ell_serial,
    "ell_omp": ell_omp,
    
    "bcsr_all": bcsr_all,
    "bcsr_serial": bcsr_serial,
    "bcsr_omp": bcsr_omp
}

for name, data in all_data.items():
    file_name = output_path + input_file + "_" + name + ".csv"
    with open(file_name, "w") as f:
        writer = csv.writer(f)
        for ln in data:
            writer.writerow(ln)
