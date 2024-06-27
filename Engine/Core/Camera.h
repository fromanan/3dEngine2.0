#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::vec3 position = glm::vec3(1, 1, 5);
	glm::vec3 offset = glm::vec3(0, 0, 0);

	glm::mat4 getViewMatrix() {
		return ViewMatrix;
	}
	glm::mat4 getProjectionMatrix() {
		return ProjectionMatrix;
	}

private:
	// horizontal angle : toward -Z
	float horizontalAngle = 3.14f;
	// vertical angle : 0, look at the horizon
	float verticalAngle = 0.0f;
	// Initial Field of View
	float initialFoV = 45.0f;
	float maxAngle = 1.5;

	float mouseSpeed = 0.005f;
	float speedWalk = 2600.0f;

};

