

##
## The S-ELL structures
##
dim = 5
block_size = 2
rowptr = [ 0, 10, 14, 16, 18 ]
colidx = [
    5, 2, 4, 2, 5,
    3, 7, 2, 0, 0,
    2, 7,
    0, 0,
    0,
    0,
    6,
    0,
]
vals = [
    5, 2, 4, 2, 5,
    3, 7, 2, 0, 0,
    7, 5,
    0, 0,
    0,
    8,
    3,
    0,
]

##
## The algorithm
##
for bi in range(dim):
    i = bi * block_size
    print(i)

