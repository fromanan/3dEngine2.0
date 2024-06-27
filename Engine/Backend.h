#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Engine/Core/Common.h"


namespace Backend {

	int init();
	GLFWwindow* GetWindowPointer();

	bool IsWindowOpen();

	void SwapBuffers();

}
