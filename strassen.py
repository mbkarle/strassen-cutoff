#python strassen 0 dimension inputfile
from random import seed
from random import randint
from sys import argv



def main():

    seed(3)

    a = matrix_filler()
    b = matrix_filler()

    print(strassen(a,b, 2))

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

def strassen(m1,m2,n0):

    size = len(m1)
    if size <= n0:
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
    p1 = strassen(a, buffer1, n0)
    add(a,b, buffer1)
    p2 = strassen(buffer1, h, n0)
    add(c,d,buffer1)
    p3 = strassen(buffer1, e, n0)
    subtract(g,e,buffer1)
    p4 = strassen(d,buffer1, n0)
    add(a,d,buffer1)
    add(e,h,buffer2)
    p5 = strassen(buffer1,buffer2, n0)
    subtract(b,d,buffer1)
    add(g,h,buffer2)
    p6 = strassen(buffer1,buffer2, n0)
    subtract(a,c,buffer1)
    add(e,f,buffer2)
    p7 = strassen(buffer1, buffer2, n0)


    #overwrite a,b,c,d becuase they are no longer being

    #let s1 = a, s2 = b, s3 = c, s4 = c

    # ae + bg = p5 + p4 - p2 + p6
    add(p5, p4, a)
    subtract(a,p2,a)
    add(a,p6,a)

    # af + bh = p1 + p2
    add(p1,p2,b)

    # ce + dg = p3 + p4
    add(p3,p4,c)

    # cf + dh = p5 + p1 - p3 - p7
    add(p5,p1,d)
    subtract(d,p3,d)
    subtract(d,p7,d)

    #s is the solution to m1 * m2

    s = [[0 for i in range(size)] for j in range(size)]

    for i in range(new_size):
        for j in range(new_size):
            s[i][j] = a[i][j]
            s[i][j + new_size] = b[i][j]
            s[i + new_size][j] = c[i][j]
            s[i + new_size][j + new_size] = d[i][j]

    return(s)


main()
