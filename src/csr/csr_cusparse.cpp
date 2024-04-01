#include "matrix.h"

#include <cuda_runtime_api.h>
#include <cusparse.h>
#include <stdio.h>
#include <stdlib.h>


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

//
// The calculation algorithm for the current format
//
double Matrix::calculate() {
    double start = getTime();
    
    uint64_t *d_rowptr, *d_rowidx;
    double *d_values, *dB, *dC;
    CHECK_CUDA( cudaMalloc((void**)&d_rowptr, rowptr, (rows+1) * sizeof(uint64_t)) )
    CHECK_CUDA( cudaMalloc((void**)&d_rowidx, rowidx, coo->nnz * sizeof(uint64_t)) )
    CHECK_CUDA( cudaMalloc((void**)&d_values, values, coo->nnz * sizeof(double)) )
    CHECK_CUDA( cudaMalloc((void**)&dB, (rows * cols) * sizeof(double)) )
    CHECK_CUDA( cudaMalloc((void**)&dC, (rows * cols) * sizeof(double)) )
    
    CHECK_CUDA( cudaMemcpy(d_rowptr, rowptr, (rows+1) * sizeof(uint64_t), cudaMemcpyHostToDevice) )
    CHECK_CUDA( cudaMemcpy(d_rowidx, rowidx, coo->nnz * sizeof(uint64_t), cudaMemcpyHostToDevice) )
    CHECK_CUDA( cudaMemcpy(d_values, values, coo->nnz * sizeof(double), cudaMemcpyHostToDevice) )
    CHECK_CUDA( cudaMemcpy(dB, B, (rows * cols) * sizeof(double), cudaMemcpyHostToDevice) )
    CHECK_CUDA( cudaMemcpy(dC, C, (rows * cols) * sizeof(double), cudaMemcpyHostToDevice) )
    
    cusparseHandle_t     handle = NULL;
    cusparseSpMatDescr_t matA;
    cusparseDnMatDescr_t matB, matC;
    void*                dBuffer    = NULL;
    size_t               bufferSize = 0;
    double  alpha       = 1.0f;
    double  beta        = 0.0f;
    
    // Create sparse matrix A in CSR format
    CHECK_CUSPARSE( cusparseCreate(&handle) )
    CHECK_CUSPARSE( cusparseCreateCsr(&matA, rows, cols, coo->nnz,
                          d_rowptr, d_rowidx, d_values,
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
    cudaFree(d_colptr);
    cudaFree(d_colidx);
    cudaFree(d_values);
    cudaFree(dB);
    cudaFree(dC);
    
    double end = getTime();
    return (double)(end-start);
}

