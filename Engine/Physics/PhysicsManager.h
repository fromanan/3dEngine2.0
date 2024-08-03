#pragma once
#include "Cube.h"
#include "Rigidbody.h"

namespace PhysicsManager
{
	RigidBody* AddRigidBody(glm::vec3 position, const std::string& name);
	Cube* AddCube(glm::vec3 position, glm::vec3 min, glm::vec3 max, const std::string& name);
	Cube* AddCube(glm::vec3 position, float width, float height, float depth, const std::string& name);
	Cube* AddCube(GameObject* gameObject, const std::string& name);

	void RemoveCube(const std::string& name);
	void RemoveRigidBody(const std::string& name);

	Cube* GetCollider(const std::string& name);
	RigidBody* GetRigidBody(const std::string& name);

	void Update(float deltaTime);
};

struct RayInfo
{
	std::string name = Tags::NOTHING;
	float distance = 9999;
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 normal = glm::vec3(0, 0, 0);
	Cube* collider = nullptr;
};
