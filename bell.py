rows = 9
blocks = 3

collens = [2, 3, 1]
colstart = [0, 6, 15]

colidx = [
    1, 2, 1, 2, 1, 2,           # 0 (bi: 0) (c: 2)
    1, 2, 3, 1, 2, 3, 1, 2, 3,  # 6  (bi: 3) (c: 3)
    1, 1, 1                     # 15 (bi: 6) (c: 1)
]

print(rows)
print(blocks)
print(collens)
print(colstart)
print(colidx)

print("----------------")

for bi in range(blocks):
    cl = collens[bi]
    cs = colstart[bi]
    print(bi, " -> ", cl, " ", cs)
    for n1 in range(blocks):
        i = bi * blocks + n1
        print("--",i)
        for n2 in range(cl):
            idx = cs+(n1*cl+n2)
            print("----", n2, " ", idx)


