/*----------Imports----------*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/*----------Function declarations----------*/
int ** matrix_memory_allocator(int size);
void free_matrix(int ** m, int size);
void add(int ** m1, int ** m2, int ** m3, int size);
void subtract(int ** m1, int ** m2, int ** m3, int size);
void standard_mm(int ** m1, int ** m2, int ** m3, int size);
void random_matrix_filler(int ** m1, int size);
void print_matrix(int ** m1, int size);
void zero_filler(int ** m1, int size);
void strassen(int ** m1, int ** m2, int ** m3, int size, int n0);
void init_matrices(int ** m1, int ** m2, int ** m3, int size);
void testCorrectness(int ** m1, int ** m2, int ** m3, int size);
void test_n0_vals(int ** m1, int ** m2, int ** m3, int size);
double timed_run(int ** m1, int ** m2, int ** m3, int size, int n0);
double multi_trial_run(int ** m1, int ** m2, int ** m3, int size, int n0, int trials);
void triagle_numbers(int ** m1, int ** m2, int ** m3, int size, double p);
int even(int n);
int loadInput(char * filename, int ** m1, int ** m2, int size);
void print_result(int ** m, int size);


/*----------Main----------*/
/*
 * Expect command line input in following format
 * ./strassen flag dimension inputfile
 */
/*------------------------*/
int main(int argc, char * argv[])
{
    /*----------Read Inputs----------*/
    int flag = atoi(argv[1]);
    int size = atoi(argv[2]);
    char * filename = argv[3];
    int n0 = 64; //empirically determined best

    //instantiate the matrix buffers
    int ** m1,** m2,** m3;

    sleep(1); //ensure new seed from last call
    srand(time(0)); //get random seed

    /*----------Allocate memory----------*/
    m1 = matrix_memory_allocator(size);
    m2 = matrix_memory_allocator(size);
    m3 = matrix_memory_allocator(size);

    /*----------Fill matrices----------*/
    if(loadInput(filename, m1, m2, size) == 0) { //attempt to load from input file, otherwise load randomly
        random_matrix_filler(m1, size);
        random_matrix_filler(m2, size);
    }
    zero_filler(m3, size); //all zeros for buffer to fill

    /*----------Run Tests depending on user flag----------*/
    switch(flag) {
        case 1: //test -> is strassen's still producing correct answer?
            testCorrectness(m1, m2, m3, size);
            break;
        case 2: //test -> what power of 2 n0 produces fastest times up to n0 = n
            test_n0_vals(m1, m2, m3, size);
            break;
        case 3: //test -> average time for multiple trials at same n0
            multi_trial_run(m1,m2,m3,size,n0,5);
            break;
        case 4: //task 3 -> triangles!
            zero_filler(m1,size);
            zero_filler(m2,size);
            triagle_numbers(m1, m2, m3, size, 0.01);
            triagle_numbers(m1, m2 ,m3, size, 0.02);
            triagle_numbers(m1, m2 ,m3, size, 0.03);
            triagle_numbers(m1, m2, m3, size, 0.04);
            triagle_numbers(m1, m2, m3, size, 0.05);
            break;
        default: //0 or unknown flag -> perform a single timed run of strassens at some n0
            strassen(m1, m2, m3, size, n0);
            print_result(m3, size);
            break;
    }

    /*----------Free Allocated Memory----------*/
    free_matrix(m1, size);
    free_matrix(m2, size);
    free_matrix(m3, size);

}

/*----------Load matrix elements into buffers----------*/
int loadInput(char * filename, int ** m1, int ** m2, int size) {
    FILE * fptr = NULL;
    fptr = fopen(filename, "r");
    if(fptr == NULL)
        return 0; //no file given
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            fscanf(fptr, "%i", &m1[i][j]);
        }
    }
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            fscanf(fptr, "%i", &m2[i][j]);
        }
    }
    fclose(fptr);
    return 1; //valid file, inputs loaded
}
        

/*----------Test that strassen implementation works----------*/
/*
 * @params m1, m2 -> filled matrices to multiply
 * m3 -> buffer to fill
 * size -> dimension of all m1, m2, m3
 */
/*-----------------------------------------------------------*/
void testCorrectness(int ** m1, int ** m2, int ** m3, int size)
{
    int ** standard_comparison = matrix_memory_allocator(size);
    standard_mm(m1, m2, standard_comparison, size);

    int n0 = 2;
    strassen(m1, m2, m3, size, n0);

    int is_equal = 1;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            if(m3[i][j] != standard_comparison[i][j]) {
                is_equal = 0;
                break;
            }
        }
    }

    char * print = (is_equal == 1) ? "correct." : "not correct.";
    printf("The strassen implementation is %s\n", print);


    free_matrix(standard_comparison, size);
}

/*----------Time Multiple Trials at n0----------*/
/*
 * special params:
 * n0 -> a specific crossover point constant for all trials
 * trials -> number of trials to time and average
 */
/*----------------------------------------------*/
double multi_trial_run(int ** m1, int ** m2, int ** m3, int size, int n0, int trials)
{
    double total_time = 0;
    for(int i = 0; i < trials; i++)
    {
        random_matrix_filler(m1, size);
        random_matrix_filler(m2, size);
        zero_filler(m3, size);

        total_time += timed_run(m1, m2, m3, size, n0);
      }
    //printf("%f average time, n0 = %i \n", total_time / (double)trials,n0);
    double average = total_time / (double)trials;
    return average;
}

/*----------Count Triangles in graph----------*/
/*
 * special params:
 * p -> ?
 */
/*--------------------------------------------*/
void triagle_numbers(int ** m1, int ** m2, int ** m3, int size, double p)
{
    float total_triangles = 0;
    for (int k = 0; k < 20; k++)
    {
        int counter = 0;
        for (int i = 1; i < size; i++)
        {
            for (int j = 0; j < i; j++)
            {
                double random = (double)rand() / (double)RAND_MAX;
                if (random <= p)
                {
                    m1[i][j] = 1;
                    m1[j][i] = 1;
                }
            }
        }

        standard_mm(m1,m1,m2,size); //?
        standard_mm(m2,m1,m3,size);

        for (int i = 0; i < size; i++)
        {
            counter += m3[i][i];
        }

        float number_of_triangles = (float)counter / (float)6;
        total_triangles += number_of_triangles;
        printf("p = %f, number of triangles = %f \n",p,number_of_triangles);
        zero_filler(m1, size);
        zero_filler(m2,size);
        zero_filler(m3,size);
    }
    float average_triangles = total_triangles / (float)20;
    printf("p = %f, average number of triangles = %f \n",p,average_triangles);
}

/*----------Test n0 values up to matrix dimension----------*/
void test_n0_vals(int ** m1, int ** m2, int ** m3, int size)
{
    int TRIALS = 1;

    for (int n0 = 32; n0 <= 256; n0++) {
        //sleep(1); //ensure new seed
        //srand(time(0));
        double average_time = multi_trial_run(m1, m2, m3, size, n0, TRIALS);
        printf("%lf,", average_time);
    }
    printf("\n");
}

/*----------Perform a single timed run of strassens at given n0----------*/
double timed_run(int ** m1, int ** m2, int ** m3, int size, int n0)
{
    clock_t start = clock();
    strassen(m1, m2, m3, size, n0);
    clock_t end = clock();

    double time_elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    return time_elapsed;
}

/*====================*/
/*      Strassen      */
/*====================*/
void strassen(int ** m1, int ** m2, int ** m3, int size, int n0)
{
    if (size <= n0)
    {
        standard_mm(m1,m2,m3,size);
    }
    else
    {
        size = even(size); //can assume matrix is properly padded, so should use even size
        int new_size = size/2 ;
        int padded_size = even(new_size);

        /*----------Allocate space for all sub arrays----------*/
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

        //fill sub arrays using m1, m2 vals
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

        //two buffers to efficiently store temporary sum and difference matrices
        int ** buffer1 = matrix_memory_allocator(padded_size);
        int ** buffer2 = matrix_memory_allocator(padded_size);
        subtract(f,h,buffer1,padded_size);
        strassen(a, buffer1, p1,padded_size, n0);
        add(a,b, buffer1,padded_size);
        strassen(buffer1, h, p2, padded_size, n0);
        add(c,d,buffer1,padded_size);
        strassen(buffer1, e, p3,padded_size, n0);
        subtract(g,e,buffer1,padded_size);
        strassen(d,buffer1, p4,padded_size, n0);
        add(a,d,buffer1,padded_size);
        add(e,h,buffer2,padded_size);
        strassen(buffer1,buffer2, p5,padded_size,n0);
        subtract(b,d,buffer1,padded_size);
        add(g,h,buffer2,padded_size);
        strassen(buffer1,buffer2,p6,padded_size, n0);
        subtract(a,c,buffer1,padded_size);
        add(e,f,buffer2,padded_size);
        strassen(buffer1, buffer2,p7,padded_size, n0);

        //overwrite a,b,c,d becuase they are no longer being

        //let s1 = a, s2 = b, s3 = c, s4 = c

        //ae + bg = p5 + p4 - p2 + p6
        add(p5, p4, a,padded_size);
        subtract(a,p2,a,padded_size);
        add(a,p6,a,padded_size);

        //af + bh = p1 + p2
        add(p1,p2,b,padded_size);

        //ce + dg = p3 + p4
        add(p3,p4,c,padded_size);

        //cf + dh = p5 + p1 - p3 - p7
        add(p5,p1,d,padded_size);
        subtract(d,p3,d,padded_size);
        subtract(d,p7,d,padded_size);

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

        free_matrix(buffer1, padded_size);
        free_matrix(buffer2, padded_size);

        free_matrix(a,padded_size);
        free_matrix(b,padded_size);
        free_matrix(c,padded_size);
        free_matrix(d,padded_size);

        free_matrix(e,padded_size);
        free_matrix(f,padded_size);
        free_matrix(g,padded_size);
        free_matrix(h,padded_size);

        free_matrix(c1,padded_size);
        free_matrix(c2,padded_size);
        free_matrix(c3,padded_size);
        free_matrix(c4,padded_size);

        free_matrix(p1,padded_size);
        free_matrix(p2,padded_size);
        free_matrix(p3,padded_size);
        free_matrix(p4,padded_size);
        free_matrix(p5,padded_size);
        free_matrix(p6,padded_size);
        free_matrix(p7,padded_size);

    }
}

/*----------Allocate 2 dimensional size x size array----------*/
int ** matrix_memory_allocator(int size)
{
    int dim = even(size);

    int ** m;

    m = (int **) malloc(dim * sizeof(int*));
    for (int i = 0; i < dim; i++)
    {
        m[i] = (int *)malloc(dim * sizeof(int));
        if(dim > size)
            m[i][size] = 0; //padding should automatically be assigned 0
    }
    if(dim > size) {
        for(int j = 0; j < dim; j++) {
            m[size][j] = 0; //padding should automatically be assigned 0
        }
    }

    return(m);
}

/*----------Free 2 dimensional size x size array m----------*/
void free_matrix(int ** m, int size)
{
    int dim = even(size); //all allocated matrices are even so can assume should free dim
    for (int i = 0; i < dim; i++)
    {
        free(m[i]);
    }
    free(m);
}

/*----------Add size x size matrices m1, m2 and store result in m3----------*/
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

/*----------Fill size x size matrix m3 = m1 - m2----------*/
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

/*----------Standard naive matrix multiplication----------*/
void standard_mm(int ** m1, int ** m2, int ** m3, int size)
{
    zero_filler(m3, size);
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

/*----------Fill size x size matrix m1 with random numbers----------*/
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

/*----------Print size x size matrix m1----------*/
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

/*----------Fill size x size buffer m1 with zeros----------*/
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

//return next even if odd
int even(int n)
{
    return (n % 2 == 0) ? n : n + 1;
}

//print only the diagonals of matrix for machine grader
void print_result(int ** m, int size) {
    for(int i = 0; i < size; i++) {
        printf("%i\n", m[i][i]);
    }
}
//ye
