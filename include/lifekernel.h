#pragma once

#include "graphics.h"

#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

__global__ void gameOfLife(unsigned char* d_c, unsigned char* d_n, uchar4* d_pb);

void initLife(unsigned char* h_grid);
void iterateLife();
void clearLife();

void setupGLpbo();
void clearGLpbo();
uchar4* map();
void syncUnmap();