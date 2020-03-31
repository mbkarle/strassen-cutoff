# strassen-cutoff

A C implementation of strassen's matrix multiplication algorithm meant to test and incorporate an optimal crossover point `n_0` from strassen's into naive multiplication. With an empirically determined `n_0 = 64`, the strassen-naive hybridization outperforms strassen's alone dramatically in practical input ranges, and outperforms naive multiplication both practically and asymptotically.

The compiled executable accepts command line inputs of the form `./strassen <test_flag> <matrix_dimension> <input_file>`, and can take a variety of test flags to test various features and generate pertinent timing data. The input file may also be omitted to instead generate random elements for multiplied matrices.

Initially produced to the constraints of an auto-grader, input sanitation and practical outputs have not yet been updated to more generalized use.
