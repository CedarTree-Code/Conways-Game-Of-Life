#include "lifekernel.h"

GLuint pbo; 
cudaGraphicsResource_t cuda_pbo_resource; 

unsigned char* d_current = nullptr;
unsigned char* d_next = nullptr;

__global__ void gameOfLife(unsigned char* d_c, unsigned char* d_n, uchar4* d_pb) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    if (x >= sim_w || y >= sim_h) return;

    //logic
    char n = 0;
    for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
            if(dx==0 && dy==0) 
                continue;
            else if(d_c[ ((y+dy+sim_h)%sim_h) * sim_w + (x+dx+sim_w)%sim_w ]==1) //proper wrapping (negative modulo fix)
                n++;
        }
    }

    int i = y * sim_w + x;

    bool alive = (d_c[i] == 0 && BORN) || (d_c[i] == 1 && SURVIVE);
    
    if(alive){
        d_pb[i] = make_uchar4(ALIVE_COLOR, 255);
        d_n[i] = 1; 
    }else{ 
        d_pb[i] = make_uchar4(DEAD_COLOR, 255); 
        d_n[i] = 0; 
    }
}

void initLife(unsigned char* h_grid) {
    size_t gridSize = sim_w * sim_h * sizeof(unsigned char);
    
    cudaMalloc(&d_current, gridSize);
    cudaMalloc(&d_next, gridSize);

    cudaMemcpy(d_current, h_grid, gridSize, cudaMemcpyHostToDevice);
    cudaMemset(d_next, 0, gridSize);
}

void iterateLife() {
    uchar4* d_pixelbuff = {}; 

    dim3 threadsPerBlock(16, 16);
    dim3 blockNum((sim_w + 15) / 16, (sim_h + 15) / 16);

    d_pixelbuff = map();
    gameOfLife<<<blockNum, threadsPerBlock>>>(d_current, d_next, d_pixelbuff);
    syncUnmap();

    drawTexture(pbo); 

    std::swap(d_current, d_next);
}

void clearLife() {
    if (d_current) {
        cudaFree(d_current);
        d_current = nullptr;
    }
    if (d_next) {
        cudaFree(d_next);
        d_next = nullptr;
    }

    clearGLpbo();
    clearGLTexture();
}

void setupGLpbo() {
    glGenBuffers(1, &pbo);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, sim_w * sim_h * sizeof(uchar4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    cudaError_t regErr = cudaGraphicsGLRegisterBuffer(&cuda_pbo_resource, pbo, cudaGraphicsRegisterFlagsWriteDiscard);
    if (regErr != cudaSuccess) {
        printf("PBO Registration Failed: %s\n", cudaGetErrorString(regErr));
    } else {
        printf("PBO Registered Successfully with CUDA!\n");
    }
}

void clearGLpbo() {
    if (cuda_pbo_resource) {
        cudaGraphicsUnregisterResource(cuda_pbo_resource);
        cuda_pbo_resource = nullptr;
    }

    if (pbo) {
        glDeleteBuffers(1, &pbo);
        pbo = 0;
    }
}

uchar4* map() {
    uchar4* d_ptr = {};
    size_t num_bytes;

    cudaError_t err = cudaGraphicsMapResources(1, &cuda_pbo_resource, 0);
    if (err != cudaSuccess) {
        printf("CUDA Map Resources Failed: %s\n", cudaGetErrorString(err));
        return nullptr;
    }

    err = cudaGraphicsResourceGetMappedPointer((void**)&d_ptr, &num_bytes, cuda_pbo_resource);
    if (err != cudaSuccess) {
        printf("CUDA Mapped Pointer Failed: %s\n", cudaGetErrorString(err));
        return nullptr;
    }
    
    return d_ptr;
}

void syncUnmap() {
    cudaDeviceSynchronize();
    cudaGraphicsUnmapResources(1, &cuda_pbo_resource, 0);
}