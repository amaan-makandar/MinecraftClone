#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"
#include "ChunkManager.h"
#include "MousePicker.h"
#include "Display/display.h"

void processInput(GLFWwindow* window);

// settings
float fov = 60.0f;
Camera camera;

void processInput(GLFWwindow* window) {   
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    camera.keyboard_input(window);
}

int main() {
    Display& display = Display::get_instance();
    GLFWwindow* window = display.get_raw_ptr();

    camera.set_cursor_callback(window, &camera);
    camera.Set_Data(fov);

    //tests whether fragments in front of other fragments
    glEnable(GL_DEPTH_TEST);

    MousePicker picker(&camera, window);

    ChunkManager chunkMgr(&camera, &picker, window);

    //-------turns on wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) {

        float currentFrame = static_cast<float>(glfwGetTime());
        camera.calc_time(currentFrame);

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
