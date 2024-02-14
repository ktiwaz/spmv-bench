/*
 * This kernel uses a global memory approach, where each thread is responsible for computing a single element in the resulting matrix. 
 * The use of thread and block indices to calculate the row and column indices helps in mapping the threads to the elements of the matrices.
 *
 */
 #include <stdio.h>
 #include <stdlib.h>

#define BLOCK_SIZE 8

#define doubleW double

__global__ void global_element(double* A, double* B, double* C, int n) {

    double C_value = 0;
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    for (int k = 0; k < n; k++) {
        C_value += A[row * n + k] * B[n * k + col];
    }

    // Each thread writes one element to C matrix
    C[row * n + col] = C_value;
}

void matmul_kernel(size_t N, double* A, double* B, double* C) {
    double *A_device, *B_device, *C_device;
    cudaMallocManaged(&A_device, N*N*sizeof(double));
    cudaMallocManaged(&B_device, N*N*sizeof(double));
    cudaMallocManaged(&C_device, N*N*sizeof(double));

    cudaMemcpy(A_device, A, N*N*sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(B_device, B, N*N*sizeof(double), cudaMemcpyHostToDevice);

    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 dimGrid(N / dimBlock.x, N / dimBlock.y);
    global_element<<<dimGrid, dimBlock>>>(A_device, B_device, C_device, N);
    cudaDeviceSynchronize();

    cudaMemcpy(C, C_device, N*N*sizeof(double), cudaMemcpyDeviceToHost);
    cudaFree(A_device);
    cudaFree(B_device);
    cudaFree(C_device);
}

int main() {
    size_t N = 2048 * 4;
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

