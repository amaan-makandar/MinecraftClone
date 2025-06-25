#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"
#include "ChunkManager.h"
#include "MousePicker.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
unsigned int SCR_WIDTH = 1920;
unsigned int SCR_HEIGHT = 1080;
float fov = 60.0f;
Camera camera;

void processInput(GLFWwindow* window) {   
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    camera.keyboard_input(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowPos(window, 0, 0);
    glfwSwapInterval(0);

    glfwMakeContextCurrent(window);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    camera.set_cursor_callback(window, &camera);
    glfwSetScrollCallback(window, scroll_callback);

    camera.Set_Data(fov);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //tests whether fragments in front of other fragments
    glEnable(GL_DEPTH_TEST);

    MousePicker picker(&camera, window);

    //Chunk testChunk(&camera, 0, 0);
    ChunkManager chunkMgr(&camera, &picker, window);

    //-------turns on wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    int numCount = 1;
    int total = 0;

    while (!glfwWindowShouldClose(window)) {
        
        processInput(window);

        glClearColor(0.455f, 0.702f, 0.820f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        picker.update();
        chunkMgr.Update_Loaded_Chunks();
        chunkMgr.Render_Chunks();
        chunkMgr.procBlockInput();


        // swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
