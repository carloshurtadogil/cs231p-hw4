#include <stdio.h> //remove if not using.

#include "util.h"//implementing

typedef struct { //example structure
    int example;
    int e_g;
} Example_Structure;

double dot_product(Mat*, Mat*, int);

/*static void example_helper_function(int n){
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
}*/

void mat_multiply(Mat *A, Mat *B, Mat *C, unsigned int threads){
    //Put your code here.
    //Remember to set the correct values for C->m, and C->n
    //after doing the multiplication.
    //example_helper_function(1000);
    int n = A->n;
    C->n = n;
    C->m = A->m;
    //printf("%d %d\n", C->n, C->m);
    int product;
    for (int i = 0; i < n; i++) {
        //printf("i = %d\n", i);
        for (int j = 0; j < n; j++) {
            //printf("j = %d: ", j);
            for (int k = 0; k < n; k++) {
                product = A->ptr[i*n + k] * B->ptr[k*n + j];
                C->ptr[i*n + j] = C->ptr[i*n + j] + product;
                //printf("%d ", (i*n + j));
            }
            //printf("\n");
        }
        //printf("\n");
    }
    return;
}