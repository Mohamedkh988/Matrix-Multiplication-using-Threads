# Multi-threaded Matrices multiplication using POSIX Threads
Code organization:
1)At first the main function reads the matrices A and B from their files and put
their values in 2D arrays.
2)Then it checks if inner dimension matches or not, and prints Error if not.
3)Then it calls each function that implements matrices multiplication and writes
their outputs in a separate file.
4)For non threaded solution, the function loops over all rows in output matrix,
loops over all columns for each row and gets the value of each element in the
output matrix.
5)For row threaded solution, the function creates threads by the number of
output matrix rows, each thread calls a function that calculate the output row
of the multiplication.
6)For element threaded solution, the function creates threads by the number of
output matrix elements, each thread calls a function that calculate the output
element of the multiplication.
# Main functions:
# 1)nonThreadedMatMult
This function loops over all rows in output matrix, loops over all columns for each
row and gets the value of each element in the output matrix.
# 2)threadedMatMultPerRow
This function creates threads by the number of output matrix rows, each thread calls
a function that calculate the output row of the multiplication. At last it checks that all
threads have terminated before it finishes.
# 3)rowMult
This function is called by threads in the row threaded version. It calculates each row
of the output matrix. It loops over columns of the provided row, and multiplies A
matrix row elements by B matrix column elements.
# 4) threadedMatMultPerElement
This function creates threads by the number of output matrix elements, each thread
calls a function that calculate the output element of the multiplication. At last it
checks that all threads have terminated before it finishes.
# 5)elementMult
This function is called by threads in the element threaded version. It calculates each
element of the output matrix. It multiplies A matrix row elements by B matrix column
elements.
# Compile and Run:
Compile: matMult.c
gcc -pthread – o matMult matMult.c
Run: ./matMult
