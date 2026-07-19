#include "test.h"

__global__ void test2() {
    printf("hello from thread %d\n", threadIdx.x);
};

void test() {
    test2<<<1, 8>>>();
    cudaDeviceSynchronize();
}