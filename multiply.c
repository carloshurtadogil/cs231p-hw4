#include <stdio.h> //remove if not using.
#include <pthread.h>
#include "util.h"//implementing

typedef struct {
    Mat *X, *Y, *Z;
    int row;
} Matrices;

#define BATCH_SIZE 64 

/* GLOBAL VARIABLES */
unsigned int current_row, max_row;
pthread_mutex_t lock;
int N;
int current_index = 0;


/**
 * Compute the multiplication of a certain row in matrices
 * A and B and store result in matrix C
 * 
 * @param matrices Struct with matrices A, B, and C
 * @param curr_row Current row to be calculated and stored
*/
void  multiply(Matrices *matrices, unsigned int curr_row) {
    Mat *A = matrices->X;
    Mat *B = matrices->Y;
    Mat *C = matrices->Z;
    int product;
    for (int i = curr_row; i < (curr_row + 1); i++) // current row
        for (int j = 0; j < N; j++)  
            for (int k = 0; k < N; k++)  {
                product = A->ptr[i*N +k] * B->ptr[k*N + j];
                C->ptr[i*N + j] += product;
            }
} 

/**
 * Grab a set of rows that have yet to be calculated
 * 
 * @param rows Rows to be calculated
 * @param size The amount of rows to calculate
*/
size_t get_calc_set(unsigned int rows[], size_t size) {
    size_t i = 0;
    int start_row;
    pthread_mutex_lock(&lock);

    if( current_index < max_row ) { // there remain rows unchecked
        start_row = current_index; // Start with current index
        current_index += BATCH_SIZE;
        pthread_mutex_unlock(&lock);
        // Get the requested batch size or as many as legal
        for (i = 0; i < size && start_row <= max_row; ++i) {
            rows[i] = start_row++;
        }
    } else {
        pthread_mutex_unlock(&lock);
    }

    return i;
}

/**
 * Consumes BATCH_SIZE consecutive rows and calculate the product at each column
 * 
 * @param arg The Matrices struct with matrices A, B, and C to be calculated
*/
void *consume_and_calculate(void* arg) {
    Matrices *matrices = (Matrices *) arg;
    unsigned int set[BATCH_SIZE];
    size_t actual_size = 0;

    while ((actual_size = get_calc_set(set, BATCH_SIZE)) > 0) {
        for (int i = 0; i < actual_size; ++i) {
            unsigned int curr_row = set[i];
            multiply(matrices, curr_row);
        }
        if (actual_size < BATCH_SIZE)
            break;
    }

    return NULL;
}

/**
 * Sequentially calculate the product of every column in every row across
 * multiple threads
 * 
 * @param A Left matrix
 * @param B Right matrix
 * @param C Matrix to store the product of A x B
 * @param threads The amount of threads allowed
*/
void mat_multiply(Mat *A, Mat *B, Mat *C, unsigned int threads){

    //Put your code here.
    //Remember to set the correct values for C->m, and C->n
    //after doing the multiplication.
    //example_helper_function(1000);
    N = A->n;
    max_row = N;
    // Set m and n in Matrix C
    C->n = N;
    C->m = A->m;
    //printf("Length: %d, Threads: %d\n", N);
    Matrices matrices = { .X = A, .Y = B, .Z = C };

    if (pthread_mutex_init(&lock, NULL) != 0)
       pthread_exit((void *)1);

    /*Create 'threads' amount of pthreads to carry out tasks*/
    pthread_t thread_arr[threads];
    for (int i = 0; i < threads; i++) {
        matrices.row = i;

        if (pthread_create(&thread_arr[i], NULL, &consume_and_calculate, (void *) &matrices) != 0)
            pthread_exit((void *)1);
    }
        
    for (int i = 0; i < threads; ++i)
        pthread_join(thread_arr[i], NULL);

    pthread_mutex_destroy(&lock);
    
    return;
}

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