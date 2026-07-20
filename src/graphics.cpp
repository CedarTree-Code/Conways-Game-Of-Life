#include "graphics.h"

GLuint tex;

void setupGLWindow(GLFWwindow*& out, const char* Title) {
    out = glfwCreateWindow(window_w, window_h, Title, NULL, NULL);
    glfwMakeContextCurrent(out);
    glfwSwapInterval(1); //Matches device refresh rate
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void setupGLTexture() {
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, sim_w, sim_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //Sharp pixels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    
    glEnable(GL_TEXTURE_2D);
}

void clearGLTexture() {
    if (tex) {
        glDeleteTextures(1, &tex);
        tex = 0;
    }
}

void drawTexture(GLuint pbo) {
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, sim_w, sim_h, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, window_w, window_h);

    //Legacy OpenGL commands
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();
}
