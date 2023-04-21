# s21_matrix
## Description 

Implementation of the matrix.h library.

- The library was developed in C language of C11 standard using gcc compiler
- The library code located in the src folder
- Full coverage of library functions code with unit-tests using the Check library
- Unit tests cover at least 80% of each function (checked using gcov)
- There is a Makefile for building the library and tests (with the targets all, clean, test, s21_matrix.a, gcov_report)
- The gcov_report target generate a gcov report in the form of an html page.
- Verifiable accuracy of the fractional part is up to 6 decimal places

### Functions

Implementation basic operations with matrices: create_matrix (creation), remove_matrix (cleaning and destruction), eq_matrix (comparison), sum_matrix (addition), sub_matrix (subtraction), mult_matrix (multiplication), mult_number (multiplication by number), transpose (transpose), determinant (calculation of determinant), calc_complements (calculation of matrix of algebraic complements), inverse_matrix (finding inverse of the matrix).