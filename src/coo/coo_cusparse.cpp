#include <cuda_runtime_api.h>
#include <cusparse.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

#define CHECK_CUDA(func)                                                       \
{                                                                              \
    cudaError_t status = (func);                                               \
    if (status != cudaSuccess) {                                               \
        printf("CUDA API failed at line %d with error: %s (%d)\n",             \
               __LINE__, cudaGetErrorString(status), status);                  \
        return EXIT_FAILURE;                                                   \
    }                                                                          \
}

#define CHECK_CUSPARSE(func)                                                   \
{                                                                              \
    cusparseStatus_t status = (func);                                          \
    if (status != CUSPARSE_STATUS_SUCCESS) {                                   \
        printf("CUSPARSE API failed at line %d with error: %s (%d)\n",         \
               __LINE__, cusparseGetErrorString(status), status);              \
        return EXIT_FAILURE;                                                   \
    }                                                                          \
}

double Matrix::calculate() {
    double start = getTime();
    
    /*for (size_t arg0 = 0; arg0<coo->nnz; arg0++) {
        size_t i = coo_rows[arg0];
        size_t j = coo_cols[arg0];
        double val = coo_vals[arg0];
        for (size_t k = 0; k<k_bound; k++) {
            C[i*cols+k] += val * B[j*cols+k];
        }
    }*/
    
    size_t *d_coo_rows, *d_coo_cols;
    double *d_coo_vals, *dB, *dC;
    CHECK_CUDA( cudaMalloc((void**)&d_coo_rows, coo->nnz * sizeof(size_t)) )
    CHECK_CUDA( cudaMalloc((void**)&d_coo_cols, coo->nnz * sizeof(size_t)) )
    CHECK_CUDA( cudaMalloc((void**)&d_coo_vals, coo->nnz * sizeof(double)) )
    CHECK_CUDA( cudaMalloc((void**)&dB, (rows * cols) * sizeof(double)) )
    CHECK_CUDA( cudaMalloc((void**)&dC, (rows * cols) * sizeof(double)) )
    
    CHECK_CUDA( cudaMemcpy(d_coo_rows, coo_rows, coo->nnz * sizeof(size_t), cudaMemcpyHostToDevice) )
    CHECK_CUDA( cudaMemcpy(d_coo_cols, coo_cols, coo->nnz * sizeof(size_t), cudaMemcpyHostToDevice) )
    CHECK_CUDA( cudaMemcpy(d_coo_vals, coo_vals, coo->nnz * sizeof(double), cudaMemcpyHostToDevice) )
    CHECK_CUDA( cudaMemcpy(dB, B, (rows * cols) * sizeof(double), cudaMemcpyHostToDevice) )
    CHECK_CUDA( cudaMemcpy(dC, C, (rows * cols) * sizeof(double), cudaMemcpyHostToDevice) )
    
    cusparseHandle_t     handle = NULL;
    cusparseSpMatDescr_t matA;
    cusparseDnMatDescr_t matB, matC;
    void*                dBuffer    = NULL;
    size_t               bufferSize = 0;
    double  alpha       = 1.0f;
    double  beta        = 0.0f;
    
    // Create sparse matrix A in COO format
    CHECK_CUSPARSE( cusparseCreate(&handle) )
    CHECK_CUSPARSE( cusparseCreateCoo(&matA, rows, cols, coo->nnz,
                          d_coo_rows, d_coo_cols, d_coo_vals,
                          CUSPARSE_INDEX_64I,
                          CUSPARSE_INDEX_BASE_ZERO, CUDA_R_64F))
    
    // Create dense matrix B
    CHECK_CUSPARSE(cusparseCreateDnMat(&matB, rows, cols, rows, dB,
                            CUDA_R_64F, CUSPARSE_ORDER_ROW))
    
    // Create dense matrix C
    CHECK_CUSPARSE(cusparseCreateDnMat(&matC, cols, rows, cols, dC,
                            CUDA_R_64F, CUSPARSE_ORDER_ROW))
    
    // Allocate external buffer if needed
    CHECK_CUSPARSE( cusparseSpMM_bufferSize(handle,
                             CUSPARSE_OPERATION_NON_TRANSPOSE,
                             CUSPARSE_OPERATION_NON_TRANSPOSE,
                             &alpha, matA, matB, &beta, matC, CUDA_R_64F,
                             CUSPARSE_SPMM_ALG_DEFAULT, &bufferSize) )
    CHECK_CUDA( cudaMalloc(&dBuffer, bufferSize) )
    
    // Execute SpMM
    CHECK_CUSPARSE( cusparseSpMM(handle,
                 CUSPARSE_OPERATION_NON_TRANSPOSE,
                 CUSPARSE_OPERATION_NON_TRANSPOSE,
                 &alpha, matA, matB, &beta, matC, CUDA_R_64F,
                 CUSPARSE_SPMM_ALG_DEFAULT, dBuffer))
    
    // Destroy matrix descriptors
    cusparseDestroySpMat(matA);
    cusparseDestroyDnMat(matB);
    cusparseDestroyDnMat(matC);
    cusparseDestroy(handle);
    
    // Copy C back
    cudaMemcpy(C, dC, (rows * cols) * sizeof(double), cudaMemcpyDeviceToHost);
    
    // Free all the memory
    cudaFree(dBuffer);
    cudaFree(d_coo_rows);
    cudaFree(d_coo_cols);
    cudaFree(d_coo_vals);
    cudaFree(dB);
    cudaFree(dC);
    
    double end = getTime();
    return (double)(end-start);
}

