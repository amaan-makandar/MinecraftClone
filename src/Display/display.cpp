#include "display.h"
#include <iostream>
#include <stdexcept>

static bool GLFW_INIT = false;
static bool OPENGL_INIT = false;

// settings
static unsigned int scr_width = 1920;
static unsigned int scr_height = 1080;
static const char* title = "Minecraft Clone";

void glfw_init();
void opengl_init();
void cleanup_with_error(const char* error_msg);
void window_resize_callback(GLFWwindow* window, int width, int height);

Display::Display(unsigned int width, unsigned int height, const char* title) {
	glfw_init();

	raw_window_ptr_ = glfwCreateWindow(scr_width, scr_height, title, NULL, NULL);
	if (!raw_window_ptr_)
		cleanup_with_error("window creation failed");

	glfwSetWindowPos(raw_window_ptr_, 0, 0);
	glfwSwapInterval(0);
	glfwSetInputMode(raw_window_ptr_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(raw_window_ptr_, window_resize_callback);
	
	glfwMakeContextCurrent(raw_window_ptr_);
	opengl_init();	// has to be called after making context current
}

Display& Display::get_instance() {
	static Display INSTANCE(scr_width, scr_height, title);
	return INSTANCE;
}

GLFWwindow* Display::get_raw_ptr() {
	return raw_window_ptr_;
}

void glfw_init() {
	if (GLFW_INIT)
		cleanup_with_error("glfw already init");
	if (!glfwInit())
		cleanup_with_error("glfw init failed");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFW_INIT = true;
}

void opengl_init() {
	if (OPENGL_INIT)
		cleanup_with_error("opengl already init");

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		cleanup_with_error("opengl init failed");

	OPENGL_INIT = true;
}

void cleanup_with_error(const char* error_msg) {
	if (GLFW_INIT)
		glfwTerminate();

	std::cout << error_msg << "\n";
	throw std::runtime_error(error_msg);
}

void window_resize_callback(GLFWwindow* window, int width, int height) {
	if (!OPENGL_INIT)
		cleanup_with_error("opengl not init in window resize fn");

	glViewport(0, 0, width, height);
	scr_width = width;
	scr_height = height;
}