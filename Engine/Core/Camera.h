#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Input.h"
#include "Physics/Physics.h"

namespace Camera
{
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	glm::vec3 GetPostion();
	void SetHorizontalAngle(float angle);
	void SetVerticalAngle(float angle);
	void SetPosition(glm::vec3 pos);


	void CheckIntersectingWithRay(Cube* cube);
	Ray GetRay();

	float GetLookingAtDistance();
	std::string GetLookingAtName();
	glm::vec3 GetRayDirection();
	glm::vec3 GetNormalFace();
	Cube* GetLookingAtCollider();
	glm::vec3 GetDirection();

	RayInfo* GetRayInfo();
	//used to get the info of the object behind the first object for windows
	RayInfo* GetRayInfo2();



	void Update(float dt);

};

