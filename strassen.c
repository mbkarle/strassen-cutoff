#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


int ** matrix_memory_allocator(int size);
void free_matrix(int ** m, int size);
void add(int ** m1, int ** m2, int ** m3, int size);
void subtract(int ** m1, int ** m2, int ** m3, int size);
void standard_mm(int ** m1, int ** m2, int ** m3, int size);
void random_matrix_filler(int ** m1, int size);
void print_matrix(int ** m1, int size);
void zero_filler(int ** m1, int size);
void strassen(int ** m1, int ** m2, int ** m3, int size, int n0);


int main(int argc, char * argv[])
{
    int n = atoi(argv[1]);
    int n0;
    double total_time;

    for (int k = 1; k <= n; k = 2*k)
    {
        sleep(1);
        srand(time(0));
        n0 = k;
        total_time = 0;
        for (int i = 0; i<5; i++)
        {

            int ** a = matrix_memory_allocator(n);
            int ** b = matrix_memory_allocator(n);
            random_matrix_filler(a,n);
            random_matrix_filler(b,n);
            //print_matrix(a,n);
            //print_matrix(b,n);
            int ** c = matrix_memory_allocator(n);
            zero_filler(c,n);

            clock_t begin = clock();
            strassen(a,b,c,n,n0);
            //standard_mm(a,b,c,n);
            //print_matrix(c,n);
            clock_t end = clock();
            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            total_time += time_spent;

            free_matrix(a,n);
            free_matrix(b,n);
            free_matrix(c,n);
          }
          printf("%f average time, n0 = %i \n", total_time / (double)5,n0);
      }
}

void strassen(int ** m1, int ** m2, int ** m3, int size, int n0)
{
    if (size <= n0)
    {
        standard_mm(m1,m2,m3,size);
    }
    else
    {
        int new_size = size/2 ;

        int ** a = matrix_memory_allocator(new_size);
        int ** b = matrix_memory_allocator(new_size);
        int ** c = matrix_memory_allocator(new_size);
        int ** d = matrix_memory_allocator(new_size);

        int ** e = matrix_memory_allocator(new_size);
        int ** f = matrix_memory_allocator(new_size);
        int ** g = matrix_memory_allocator(new_size);
        int ** h = matrix_memory_allocator(new_size);

        int ** c1 = matrix_memory_allocator(new_size);
        int ** c2 = matrix_memory_allocator(new_size);
        int ** c3 = matrix_memory_allocator(new_size);
        int ** c4 = matrix_memory_allocator(new_size);

        int ** p1 = matrix_memory_allocator(new_size);
        int ** p2 = matrix_memory_allocator(new_size);
        int ** p3 = matrix_memory_allocator(new_size);
        int ** p4 = matrix_memory_allocator(new_size);
        int ** p5 = matrix_memory_allocator(new_size);
        int ** p6 = matrix_memory_allocator(new_size);
        int ** p7 = matrix_memory_allocator(new_size);


        for (int i = 0; i < new_size; i++)
        {
            for (int j = 0; j < new_size; j++)
            {
              a[i][j] = m1[i][j];
              b[i][j] = m1[i][j + new_size];
              c[i][j] = m1[i + new_size][j];
              d[i][j] = m1[i + new_size][j + new_size];

              e[i][j] = m2[i][j];
              f[i][j] = m2[i][j + new_size];
              g[i][j] = m2[i + new_size][j];
              h[i][j] = m2[i + new_size][j + new_size];
            }
        }




        int ** buffer1 = matrix_memory_allocator(new_size);
        int ** buffer2 = matrix_memory_allocator(new_size);
        subtract(f,h,buffer1,new_size);
        strassen(a, buffer1, p1,new_size, n0);
        add(a,b, buffer1,new_size);
        strassen(buffer1, h, p2, new_size, n0);
        add(c,d,buffer1,new_size);
        strassen(buffer1, e, p3,new_size, n0);
        subtract(g,e,buffer1,new_size);
        strassen(d,buffer1, p4,new_size, n0);
        add(a,d,buffer1,new_size);
        add(e,h,buffer2,new_size);
        strassen(buffer1,buffer2, p5,new_size,n0);
        subtract(b,d,buffer1,new_size);
        add(g,h,buffer2,new_size);
        strassen(buffer1,buffer2,p6,new_size, n0);
        subtract(a,c,buffer1,new_size);
        add(e,f,buffer2,new_size);
        strassen(buffer1, buffer2,p7,new_size, n0);

        //overwrite a,b,c,d becuase they are no longer being

        //let s1 = a, s2 = b, s3 = c, s4 = c

        //ae + bg = p5 + p4 - p2 + p6
        add(p5, p4, a,new_size);
        subtract(a,p2,a,new_size);
        add(a,p6,a,new_size);


        //af + bh = p1 + p2
        add(p1,p2,b,new_size);

        //ce + dg = p3 + p4
        add(p3,p4,c,new_size);

        //cf + dh = p5 + p1 - p3 - p7
        add(p5,p1,d,new_size);
        subtract(d,p3,d,new_size);
        subtract(d,p7,d,new_size);

        //s is the solution to m1 * m2

        for (int i = 0; i < new_size; i++)
        {
            for (int j = 0; j < new_size; j++)
            {
              m3[i][j] = a[i][j];
              m3[i][j + new_size] = b[i][j];
              m3[i + new_size][j] = c[i][j];
              m3[i + new_size][j + new_size] = d[i][j];
            }
        }


        free_matrix(a,new_size);
        free_matrix(b,new_size);
        free_matrix(c,new_size);
        free_matrix(d,new_size);

        free_matrix(e,new_size);
        free_matrix(f,new_size);
        free_matrix(g,new_size);
        free_matrix(h,new_size);

        free_matrix(c1,new_size);
        free_matrix(c2,new_size);
        free_matrix(c3,new_size);
        free_matrix(c4,new_size);

        free_matrix(p1,new_size);
        free_matrix(p2,new_size);
        free_matrix(p3,new_size);
        free_matrix(p4,new_size);
        free_matrix(p5,new_size);
        free_matrix(p6,new_size);
        free_matrix(p7,new_size);

    }
}

int ** matrix_memory_allocator(int size)
{
    int ** m;

    m = (int **) malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++)
    {
        m[i] = (int *)malloc(size * sizeof(int));
    }

    return(m);
}

void free_matrix(int ** m, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(m[i]);
    }
    free(m);
}

void add(int ** m1, int ** m2, int ** m3, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j ++)
        {
            m3[i][j] = m1[i][j] + m2[i][j];
        }
    }
}

void subtract(int ** m1, int ** m2, int ** m3, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j ++)
        {
            m3[i][j] = m1[i][j] - m2[i][j];
        }
    }
}

void standard_mm(int ** m1, int ** m2, int ** m3, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int k = 0; k < size; k++)
        {
            for (int j = 0; j < size; j++)
            {
                m3[i][j] = m3[i][j] + m1[i][k]*m2[k][j];
            }
        }
    }
}

void random_matrix_filler(int ** m1, int size)
{
    //generates random numbers -5 to 5 essentially
    int max_rand = 11;
    for (int i = 0; i < size; i++)
    {
      for (int j = 0; j < size; j++)
      {
          m1[i][j] = ((int)rand() % max_rand ) - 5 ;
      }
    }
}

void print_matrix(int ** m1, int size)
{
    for (int i = 0; i < size; i ++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%i ", m1[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void zero_filler(int ** m1, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            m1[i][j] = 0;
        }
    }
}

//void strassen(int ** m1, int ** m2, int n0)
