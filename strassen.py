#python strassen 0 dimension inputfile
from random import seed
from random import randint
from sys import argv



def main():

    seed(3)

     a = matrix_filler()
    b = matrix_filler()

    subtract(a,b)

#function to help testing - fills matrix with random integers from 1 to 5
def matrix_filler():
    size = int(argv[2])
    m = [[randint(0,5) for x in range(size)] for y in range(size)]
    print(m)
    return(m)

#standar matrix multiplication algorithm
def standard_mm(a,b):
    size = len(a)
    m = [[0 for x in range(size)] for y in range(size)]

    for i in range(size):
        for j in range(size):
            for k in range(size):
                m[i][j] += a[i][k] * b[k][j]

    return(m)

#adds two matrices
def add(m1,m2, to_fill):
    size = len(m1)
    for  i in range(size):
        for j in range(size):
            #avoids having to allocate memory for a new matrix
            to_fill[i][j] = m1[i][j] + m2[i][j]

#calculates a - b
def subtract(m1,m2, to_fill):
    size = len(m1)
    for  i in range(size):
        for j in range(size):
            #avoids having to allocate memory for a new matrix
            to_fill[i][j] = m1[i][j] - m2[i][j]

def strassen(m1,m2,n_0):

    size = len(m1)
    if size <= n_0:
        return (standard_mm(m1,m2))

    new_size = size/2

    # [ [a, b]    [ [e, f]     =    [ [s1, s2]
    #   [c, d] ]    [g, h] ]          [s3, s4] ]

    a = [[0 for i in range(new_size)] for j in range(new_size)]
    b = [[0 for i in range(new_size)] for j in range(new_size)]
    c = [[0 for i in range(new_size)] for j in range(new_size)]
    d = [[0 for i in range(new_size)] for j in range(new_size)]

    e = [[0 for i in range(new_size)] for j in range(new_size)]
    f = [[0 for i in range(new_size)] for j in range(new_size)]
    g = [[0 for i in range(new_size)] for j in range(new_size)]
    h = [[0 for i in range(new_size)] for j in range(new_size)]

     for i in range(new_size):
         for j in range(new_size):
             a[i][j] = m1[i][j]
             b[i][j] = m1[i][j + new_size]
             c[i][j] = m1[i + new_size][j]
             d[i][j] = m1[i + new_size][j + new_size]

             e[i][j] = m2[i][j]
             f[i][j] = m2[i][j + new_size]
             g[i][j] = m2[i + new_size][j]
             h[i][j] = m2[i + new_size][j + new_size]

    buffer1 = [[0 for i in range(new_size)] for j in range(new_size)]
    buffer2 = [[0 for i in range(new_size)] for j in range(new_size)]
    subtract(f,h,buffer1)
    p1 = strassen(a, buffer1)
    add(a,b, buffer1)
    p2 = strassen(buffer1, h)
    add(c,d,buffer1)
    p3 = strassen(buffer1, e)
    subtract(g,e,buffer1)
    p4 = strassen(d,buffer1)
    add(a,d,buffer1)
    add(e,h,buffer2)
    p5 = strassen(buffer1,buffer2)
    subtract(b,d,buffer1)
    add(g,h,buffer2)
    p6 = strassen(buffer1,buffer2)
    subtract(a,c,buffer1)
    add(e,f,buffer2)
    p7 = strassen(buffer1, buffer2)

    # (s1) ae + bg = P5 + P4 − P2 + P6
    # (s2) af + bh = P1 + P2
    # (s3) ce + dg = P3 + P4
    # (s4) cf + dh = P5 + P1 − P3 − P1

    









main()
