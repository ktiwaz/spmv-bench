rows = 9
blocks = 3

collens = [2, 3, 1]

colidx = [
    1, 2, 1, 2, 1, 2,           # 0 (bi: 0) (c: 2)
    1, 2, 3, 1, 2, 3, 1, 2, 3,  # 6  (bi: 3) (c: 3)
    1, 1, 1                     # 15 (bi: 6) (c: 1)
]

for bi in range(blocks):
    ni = bi * blocks
    for i in range(ni, ni+blocks):
        col = collens[bi]
        for c in range(col):
            idx = i * col + c
            j = colidx[idx]
            print(i, " ", j)

