#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Engine/Core/GameObject.h"

#include <vector>

class Cube
{
public:
	Cube(glm::vec3 postion, float width, float height, float depth, const char* name);
	Cube(glm::vec3 postion, glm::vec3 min, glm::vec3 max, const char* name);

	const char* getName();

	void setPosition(glm::vec3 position);
	glm::vec3 getPosition();

	bool TouchingLeft(Cube* colider, float offset);
	bool TouchingRight(Cube* colider, float offset);
	bool TouchingFront(Cube* colider, float offset);
	bool TouchingBack(Cube* colider, float offset);
	bool TouchingBottom(Cube* colider, float offset);
	bool TouchingTop(Cube* colider, float offset);

	Cube(GameObject gameobject, const char* names);

private:
	const char* name = "None";
	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 position;
	float width, depth, height;
	bool isTrigger = false;
};

class Ray {
public:
	glm::vec3 direction;
	glm::vec3 origin;

	Ray(glm::vec3 dir, glm::vec3 org);

	bool intersectsTriangle(std::vector<glm::vec3> verticies, glm::mat4 ModelMatrix);

};

