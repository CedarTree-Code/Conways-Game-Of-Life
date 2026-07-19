#include <iostream>

#include "const.h" 
#include "test.h"

struct Pixel {
    unsigned char r, g, b;
};

const int SIM_WIDTH = 128;
const int SIM_HEIGHT = 72;

Pixel pixelBuffer[SIM_WIDTH*SIM_HEIGHT];

void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    if (x >= 0 && x < SIM_WIDTH && y >= 0 && y < SIM_HEIGHT) {
        int index = y * SIM_WIDTH + x; // Flat array index formula
        pixelBuffer[index] = {r, g, b};
    }
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Pixel Grid", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    unsigned int textureColorBuffer;
    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);

    // Allocate a blank 320x240 canvas texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SIM_WIDTH, SIM_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    // Keep pixels sharp (Pixel-Art / Retro filtering)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glEnable(GL_TEXTURE_2D);

    while(!glfwWindowShouldClose(window)) {
        std::fill_n(pixelBuffer, SIM_WIDTH * SIM_HEIGHT, Pixel{0, 0, 0});

        setPixel(64, 36, 255, 255, 255); 

        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SIM_WIDTH, SIM_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixelBuffer);

        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, 1280, 720);

        // Draw a full-screen rectangle using Legacy OpenGL commands
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
        glEnd();

        // Push the image to the monitor
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    std::cout << H << std::endl;

    test();
}