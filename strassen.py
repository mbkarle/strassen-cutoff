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
def add(a,b):
    size = len(a)
    for  i in range(size):
        for j in range(size):
            #avoids having to allocate memory for a new matrix
            a[i][j] = a[i][j] + b[i][j]
    return(a)

#calculates a - b
def subtract(a,b):
    size = len(a)
    for  i in range(size):
        for j in range(size):
            #avoids having to allocate memory for a new matrix
            a[i][j] = a[i][j] - b[i][j]
    return(a)



main()
