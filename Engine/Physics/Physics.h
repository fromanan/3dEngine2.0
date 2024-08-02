#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Engine/Core/GameObject.h"

#include <vector>
#include <string>

#include "Tags.h"

class Ray
{
public:
	glm::vec3 direction;
	glm::vec3 origin;
	Ray();
	Ray(glm::vec3 dir, glm::vec3 org);
	void UpdateRay(glm::vec3 dir, glm::vec3 org);
	bool intersectsTriangle(std::vector<glm::vec3> vertices, glm::mat4 ModelMatrix) const;
};

class Cube
{
public:
	Cube(glm::vec3 position, float width, float height, float depth, std::string name);
	Cube(glm::vec3 position, glm::vec3 min, glm::vec3 max, std::string name);

	std::string GetName();
	std::string GetTag();
	void SetTag(const std::string& tag);

	void setPosition(glm::vec3 position);
	glm::vec3 getPosition();
	float getDepth() const;
	float getHeight() const;
	float getWidth() const;
	void setDimensions(float width, float height, float depth);

	glm::vec3 getMin() const;
	glm::vec3 getMax() const;

	bool GetIsTrigger() const;
	void SetIsTrigger(bool trigger);
	void SetStatic(bool isStatic);
	bool GetStatic() const;

	void SetDelete(bool Delete);
	bool ShouldDelete() const;
	
	bool TouchingLeft(const Cube* collider, float velocity) const;
	bool TouchingRight(const Cube* collider, float velocity) const;
	bool TouchingFront(const Cube* collider, float velocity) const;
	bool TouchingBack(const Cube* collider, float velocity) const;
	bool TouchingBottom(const Cube* collider, const float velocity) const;
	bool TouchingTop(const Cube* collider, const float velocity) const;

	float intersect(const Ray& r, float t0, float t1) const;

	Cube(GameObject* gameObject, std::string name);
	void Regenerate(GameObject* gameObject);

private:
	std::string name = "None";
	std::string tag = "None";
	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 position;
	float width, depth, height;
	bool isTrigger = false;
	bool isStatic = true;
	bool shouldDelete = false;
};

class RigidBody
{
public:
	RigidBody();
	RigidBody(glm::vec3 position, const std::string& name);
	glm::vec3 GetPosition() const;
	void SetPosition(glm::vec3 position);
	void SetCollider(const std::string& colliderName);
	std::string GetCollider();

	std::string GetName();

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
	std::string name = "None";
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 velocity = glm::vec3(0,0,0);
	std::string collider = "None";
};

namespace PhysicsManager
{
	RigidBody* AddRigidbody(glm::vec3 position, const std::string& name);
	Cube* AddCube(glm::vec3 position, glm::vec3 min, glm::vec3 max, const std::string& name);
	Cube* AddCube(glm::vec3 position, float width, float height, float depth, const std::string& name);
	Cube* AddCube(GameObject* gameObject, const std::string& name);

	void RemoveCube(std::string name);
	void RemoveRigidbody(std::string name);

	Cube* GetCollider(std::string name);
	RigidBody* GetRigidbody(std::string name);

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
