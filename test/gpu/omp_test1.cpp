/*
 * This kernel uses a global memory approach, where each thread is responsible for computing a single element in the resulting matrix. 
 * The use of thread and block indices to calculate the row and column indices helps in mapping the threads to the elements of the matrices.
 *
 */
 #include <stdio.h>
 #include <stdlib.h>

void matmul_kernel(int N, double *A, double *B, double *C) {
    int size = N * N;
    int i, j, k;
    #pragma omp target parallel for map(to: N, A[0:size], B[0:size]) map(from: C[0:size])
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            double temp = 0;
            for (k = 0; k < N; k++) {
                temp += (A[i * N + k] * B[k * N + j]);
            }
            C[i * N + j] = temp;
        }
    }
}

int main() {
    size_t N = 1024;
    double *A = (double *)malloc(sizeof(double)*N*N);
    double *B = (double *)malloc(sizeof(double)*N*N);
    double *C = (double *)malloc(sizeof(double)*N*N);
    for (size_t i = 0; i<N; i++) {
        A[i] = rand() % 101;
        B[i] = 1.7;
        C[i] = 0;
    }
    
    matmul_kernel(N, A, B, C);
}

