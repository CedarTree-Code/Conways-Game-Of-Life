#include <iostream>

#include "lifekernel.h"
#include "lifeforms.h"

#ifdef _WIN32
#include <windows.h>
extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif

#define isOpen(w) !glfwWindowShouldClose(w)

int main() {
    glfwInit();

    GLFWwindow* window;
    setupGLWindow(window, "Pixel Grid");
    setupGLpbo();
    setupGLTexture();

    unsigned char* h_grid = new unsigned char[sim_w*sim_h];
    randomLife(h_grid, 0.2);
    initLife(h_grid);
    delete[] h_grid;

    while(isOpen(window)) {
        iterateLife();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    clearLife();

    glfwDestroyWindow(window);
    glfwTerminate();
}