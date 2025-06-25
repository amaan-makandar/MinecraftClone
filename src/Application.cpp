#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"
#include "ChunkManager.h"
#include "MousePicker.h"
#include "Display/display.h"
#include "Math/frustum.h"

void processInput(GLFWwindow* window, Camera& camera);

void processInput(GLFWwindow* window, Camera& camera) {   
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    camera.keyboard_input(window);
}

int main() {
    Display& display = Display::get_instance();
    GLFWwindow* window = display.get_raw_ptr();

    Camera camera;
    camera.set_cursor_callback(window, &camera);

    //tests whether fragments in front of other fragments
    glEnable(GL_DEPTH_TEST);

    ChunkManager chunkMgr(&camera, window);

    //-------turns on wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (display.is_alive()) {

        camera.update();

        processInput(window, camera);

        glClearColor(0.455f, 0.702f, 0.820f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
        chunkMgr.Update_Loaded_Chunks();
        chunkMgr.Render_Chunks();

        // swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        display.update();

        Frustum f = get_frustum_from_camera(camera);
        f.print();
    }

    glfwTerminate();
    return 0;
}
