cmake_minimum_required(VERSION 3.28)

project(CUDAOpenGLInterop LANGUAGES CXX C CUDA)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CUDA_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

set(OPENGL_ROOT_DIR "C:/OpenGL_Deps")
find_path(GLAD_INCLUDE_DIR NAMES glad/glad.h PATHS "${OPENGL_ROOT_DIR}/include")
find_path(GLFW_INCLUDE_DIR NAMES GLFW/glfw3.h PATHS "${OPENGL_ROOT_DIR}/include")
find_library(GLFW_LIBRARY NAMES glfw3 PATHS "${OPENGL_ROOT_DIR}/lib")

add_library(glad_lib STATIC src/glad.c)
target_include_directories(glad_lib PUBLIC 
    ${GLAD_INCLUDE_DIR}
)

add_executable(main
    src/main.cpp
    src/lifekernel.cu
    src/graphics.cpp
    src/lifeforms.cpp
)

find_package(OpenGL REQUIRED)
find_package(CUDAToolkit REQUIRED)

set_target_properties(main PROPERTIES
    CUDA_ARCHITECTURES "89"
    CUDA_SEPARABLE_COMPILATION ON
)

target_include_directories(main PRIVATE
    ${GLFW_INCLUDE_DIR}
    ${GLAD_INCLUDE_DIR}
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(main PRIVATE
    glad_lib
    OpenGL::GL
    OpenGL::GLU
    ${GLFW_LIBRARY}
    CUDA::cudart
    CUDA::curand
)
