#pragma once

#include <iostream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

void test();
__global__ void test2();