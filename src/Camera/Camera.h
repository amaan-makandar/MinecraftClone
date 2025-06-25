#pragma once
#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

class Camera
{
private:
	//camera
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	float near_z;
	float far_z;

	//mouse controls
	bool firstMouse;
	float yaw;
	float pitch;
	float lastX;
	float lastY;
	float fov;

	//time calculations
	static float deltaTime;
	static float lastFrame;

public:


	Camera();

	//mouse callback
	void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
	//keyboard input
	void keyboard_input(GLFWwindow* window);
	//set cursor callback
	void set_cursor_callback(GLFWwindow* window, Camera* camera);

	void update();

	inline float getFrameTime() { return deltaTime; }
	inline glm::vec3 getPosition() { return cameraPos; }

	//MVP matrix getters
	glm::mat4 getPerspectiveMatrix();
	glm::mat4 getViewMatrix();
	glm::vec3 getCameraFront();

	float get_near_z();
	float get_far_z();
};