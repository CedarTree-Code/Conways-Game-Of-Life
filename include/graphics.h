#pragma once

#include <cstdio>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "const.h"

void setupGLWindow(GLFWwindow*& out, const char* Title);
void setupGLTexture();
void clearGLTexture();
void drawTexture(GLuint pbo);