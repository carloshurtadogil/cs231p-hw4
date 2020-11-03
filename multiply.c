#include <stdio.h> //remove if not using.

#include "util.h"//implementing

typedef struct { //example structure
    int example;
    int e_g;
} Example_Structure;

double dot_product(Mat*, Mat*, int);

static void example_helper_function(int n){
    // Functions defined with the modifier 'static' are only visible
    // to other functions in this file. They cannot be called from
    // outside (for example, from main.c). Use them to organize your
    // code. Remember that in C, you cannot use a function until you
    // declare it, so declare all your utility functions above the
    // point where you use them.
    //
    // Maintain the mat_sq_trans_xt functions as lean as possible
    // because we are measuring their speed. Unless you are debugging,
    // do not print anything on them, that consumes precious time.
    //
    // You may delete this example helper function and structure, and
    // write your own useful ones.

    Example_Structure es1;
    es1.example = 13;
    es1.e_g = 7;
    printf("n = %d\n", es1.example + es1.e_g + n);
    return;
}

void mat_multiply(Mat *A, Mat *B, Mat *C, unsigned int threads){
    //Put your code here.
    //Remember to set the correct values for C->m, and C->n
    //after doing the multiplication.
    //example_helper_function(1000);
    return;
}

/**
 ** Calculate and return the dot product of two pointer arrays
 ** 
 ** @param X Pointer array
 ** @param Y Pointer array
 ** @param n Size of both arrays
 ** @return The dot product of X and Y
 **/
double dot_product(Mat *X, Mat *Y, int n) {
    double product = 0.0;

    // product = product + X[i] + Y[i]
    for (int i = 0; i < n; i++) {
        product = product + *(X->ptr + i) * *(Y->ptr + i);
    }

    return product;
}