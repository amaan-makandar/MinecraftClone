#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Display {
private:
	GLFWwindow* raw_window_ptr_;

	Display(unsigned int width, unsigned int height, const char* title);

public:
	Display(Display const&) = delete;
	void operator=(Display const&) = delete;

	static Display& get_instance();
	GLFWwindow* get_raw_ptr();
	void update();
	bool is_alive();
};