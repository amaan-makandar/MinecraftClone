#include "Camera.h"

static float camera_fov = 60.0f;

float Camera::deltaTime = 0.0f;
float Camera::lastFrame = 0.0f;

Camera::Camera()
	: cameraPos(glm::vec3(0.0f, 64.0f, 1.0f)),
	cameraFront(glm::vec3(0.0f, -1.0f, 0.0f)),
	cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	firstMouse(true),
	yaw(-90.0f),
	pitch(0.0f),
	lastX(0.0f),
	lastY(0.0f),
    fov(camera_fov),
    near_z(0.1f),
    far_z(10000.0f)
{
}

void Camera::keyboard_input(GLFWwindow* window)
{
    //change this to "glm::normalize(glm::vec3(cameraFront.x, 16.0f, cameraFront.z));"
    //for true FPS camera
    float cameraSpeed = static_cast<float>(15 * deltaTime);
    glm::vec3 flatFront = cameraFront;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * flatFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * flatFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
}

void Camera::set_cursor_callback(GLFWwindow* window, Camera* camera)
{
    glfwSetWindowUserPointer(window, camera);
    //check whether this works
    auto func = [](GLFWwindow* w, double xposIn, double yposIn) {
        static_cast<Camera*>(glfwGetWindowUserPointer(w))->mouse_callback(w, xposIn, yposIn);
    };

    glfwSetCursorPosCallback(window, func);
}

void Camera::update()
{
    float curr_time = static_cast<float>(glfwGetTime());
    deltaTime = curr_time - lastFrame;
    lastFrame = curr_time;
}


void Camera::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sens = 0.1f;
    xoffset *= sens;
    yoffset *= sens;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(front);
}

glm::mat4 Camera::getPerspectiveMatrix() {
    // viewport_data: [..., ..., scr_width, scr_height]
    GLint viewport_data[4];
    glGetIntegerv(GL_VIEWPORT, viewport_data);

    return glm::perspective(glm::radians(fov), (float)viewport_data[2] / (float)viewport_data[3], near_z, far_z);
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::vec3 Camera::getCameraFront() {
    return cameraFront;
}

float Camera::get_near_z() {
    return near_z;
}

float Camera::get_far_z() {
    return far_z;
}