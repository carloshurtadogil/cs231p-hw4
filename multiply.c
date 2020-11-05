#include <stdio.h> //remove if not using.
#include <pthread.h>
#include "util.h"//implementing

typedef struct {
    Mat *X, *Y, *Z;
    int row;
} Matrices;

#define BATCH_SIZE 1

/* GLOBAL VARIABLES */
unsigned int current_row, max_row;
pthread_mutex_t lock;
int N;
int current_index = 0;


void* multiply(void* arg) 
{ 
    Matrices *matrices = (Matrices *) arg;
    Mat *A = matrices->X;
    Mat *B = matrices->Y;
    Mat *C = matrices->Z;
    int curr_i = current_index++; 

    int product;
    
    pthread_mutex_lock(&lock);
    for (int i = curr_i; i < (curr_i + 1); i++)  
        for (int j = 0; j < N; j++)  
            for (int k = 0; k < N; k++)  {
                product = A->ptr[i*N +k] * B->ptr[k*N + j];
                C->ptr[i*N + j] += product;
            }
    pthread_mutex_unlock(&lock);
    return NULL;
} 

void mat_multiply(Mat *A, Mat *B, Mat *C, unsigned int threads){

    //Put your code here.
    //Remember to set the correct values for C->m, and C->n
    //after doing the multiplication.
    //example_helper_function(1000);
    N = A->n;
    // Set m and n in Matrix C
    C->n = N;
    C->m = A->m;

    Matrices matrices = { .X = A, .Y = B, .Z = C };

    if (pthread_mutex_init(&lock, NULL) != 0)
       pthread_exit((void *)1);

    /*Create 'threads' amount of pthreads to carry out tasks*/
    pthread_t thread_arr[threads];
    for (int i = 0; i < N; i++) {
        matrices.row = i;

        if (pthread_create(&thread_arr[i], NULL, &multiply, (void *) &matrices) != 0)
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