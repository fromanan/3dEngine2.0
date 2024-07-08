#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Engine/Core/GameObject.h"

#include <vector>
#include <string>
class Ray {
public:
	glm::vec3 direction;
	glm::vec3 origin;
	Ray();
	Ray(glm::vec3 dir, glm::vec3 org);
	void UpdateRay(glm::vec3 dir, glm::vec3 org);
	bool intersectsTriangle(std::vector<glm::vec3> verticies, glm::mat4 ModelMatrix);

};

class Cube
{
public:
	Cube(glm::vec3 postion, float width, float height, float depth, std::string name);
	Cube(glm::vec3 postion, glm::vec3 min, glm::vec3 max, std::string name);

	std::string GetName();

	void setPosition(glm::vec3 position);
	glm::vec3 getPosition();
	float getDepth();
	float getHeight();
	float getWidth();
	glm::vec3 getMin();
	glm::vec3 getMax();


	bool GetIsTrigger();
	void SetIsTrigger(bool trigger);
	void SetStatic(bool Static);
	bool GetStatic();

	
	bool TouchingLeft(Cube* colider, float velocity);
	bool TouchingRight(Cube* colider, float velocity);
	bool TouchingFront(Cube* colider, float velocity);
	bool TouchingBack(Cube* colider, float velocity);
	bool TouchingBottom(Cube* colider, float velocity);
	bool TouchingTop(Cube* colider, float velocity);

	float intersect(Ray r, float t0, float t1);


	Cube(GameObject* gameobject, std::string names);

private:
	std::string name = "None";
	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 position;
	float width, depth, height;
	bool isTrigger = false;
	bool Static = true;
};
class RigidBody {
public:
	RigidBody();
	RigidBody(glm::vec3 position,const char* name);
	glm::vec3 GetPostion();
	void SetPostion(glm::vec3 position);
	void SetColider(Cube* colider);
	Cube* GetColider();

	const char* GetName();

	void NewPosition(float deltaTime);
	void NewPositionY(float deltaTime);


	void AddForce(glm::vec3 force);
	void AddForceX(float force);
	void AddForceY(float force);
	void AddForceZ(float force);

	void SetForce(glm::vec3 force);
	void SetForceX(float force);
	void SetForceY(float force);
	void SetForceZ(float force);

	void RemoveForceX();
	void RemoveForceY();
	void RemoveForceZ();

	glm::vec3 GetForce();

private:
	const char* name = "None";
	glm::vec3 position;
	glm::vec3 velocity;
	Cube* colider = NULL;
};



namespace PhysicsManager {
	RigidBody* AddRigidbody(glm::vec3 position, const char* name);
	Cube* AddCube(glm::vec3 postion, glm::vec3 min, glm::vec3 max, std::string name);
	Cube* AddCube(glm::vec3 postion, float width, float height, float depth, std::string name);
	Cube* AddCube(GameObject* gameobject, std::string name);


	Cube* GetColider(std::string name);
	RigidBody* GetRigidbody(const char* name);


	void Update(float deltaTime);
};
