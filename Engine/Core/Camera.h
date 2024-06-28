#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Input.h"

class Camera
{
public:
	glm::vec3 position = glm::vec3(1, 1, 5);
	glm::vec3 offset = glm::vec3(0, 0, 0);

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	void Update(float dt);

private:
	// horizontal angle : toward -Z
	float horizontalAngle = 3.14f;
	float verticalAngle = 0.0f;
	float initialFoV = 45.0f;
	float maxAngle = 1.5;

	float mouseSpeed = 0.005f;
	float speedWalk = 2600.0f;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

};

