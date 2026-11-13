
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 20
#define NUM_THREADS 10

int matA[MAX][MAX];
int matB[MAX][MAX];
int matSUM[MAX][MAX];
int matDIFFERENCE[MAX][MAX];
int matPRODUCT[MAX][MAX];

typedef struct {
    int start_row;
    int end_row;
} ThreadData;

// Initialize matrices with random values
void initialize_matrices() {
    srand(time(NULL));

    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matA[i][j] = rand() % 100;
            matB[i][j] = rand() % 100;
        }
    }
}

// Print a matrix
void print_matrix(int matrix[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            printf("%5d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Matrix addition
void *matrix_add(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    for (int i = data->start_row; i < data->end_row; i++) {
        for (int j = 0; j < MAX; j++) {
            matSUM[i][j] = matA[i][j] + matB[i][j];
        }
    }

    return NULL;
}

// Matrix subtraction
void *matrix_subtract(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    for (int i = data->start_row; i < data->end_row; i++) {
        for (int j = 0; j < MAX; j++) {
            matDIFFERENCE[i][j] = matA[i][j] - matB[i][j];
        }
    }

    return NULL;
}

// Matrix multiplication (dot product)
void *matrix_multiply(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    for (int i = data->start_row; i < data->end_row; i++) {
        for (int j = 0; j < MAX; j++) {
            matPRODUCT[i][j] = 0;

            for (int k = 0; k < MAX; k++) {
                matPRODUCT[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }

    return NULL;
}

// Execute an operation using 10 threads
void run_operation(void *(*operation)(void *)) {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start_row = i * (MAX / NUM_THREADS);
        thread_data[i].end_row = (i + 1) * (MAX / NUM_THREADS);

        pthread_create(&threads[i], NULL, operation, &thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    initialize_matrices();

    printf("\nInitial Matrix A:\n");
    print_matrix(matA);

    printf("\nInitial Matrix B:\n");
    print_matrix(matB);

    printf("\nPerforming Matrix Addition...\n");
    run_operation(matrix_add);

    printf("\nMatrix Addition Result:\n");
    print_matrix(matSUM);

    printf("\nPerforming Matrix Subtraction...\n");
    run_operation(matrix_subtract);

    printf("\nMatrix Subtraction Result:\n");
    print_matrix(matDIFFERENCE);

    printf("\nPerforming Matrix Multiplication...\n");
    run_operation(matrix_multiply);

    printf("\nMatrix Multiplication Result:\n");
    print_matrix(matPRODUCT);

    printf("\nAll matrix operations completed successfully!\n");

    return 0;
}
