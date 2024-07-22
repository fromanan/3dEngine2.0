#pragma once
#include "bullet/btBulletDynamicsCommon.h"
#include "bullet/LinearMath/btVector3.h"
#include "bullet/LinearMath/btAlignedObjectArray.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Engine/Core/GameObject.h"


enum ColliderShape {
	Box,
	Sphere,
	Cylinder
};


class Collider {
public:
	Collider();
	~Collider();

	Collider(GameObject* gameobject,std::string Name, float Mass);
	Collider(glm::vec3 position, float width, float height, float depth,float mass, std::string Name, ColliderShape shape);

	std::string GetName();
	std::string GetTag();
	void SetTag(std::string Tag);

	void SetPosition(glm::vec3 position);
	glm::vec3 GetPosition();
	glm::vec3 GetRotation();

	void SetDepth();
	void SetHeight();
	void SetWidth();
	void SetDimensions(float width, float height, float depth);
	void Regenerate(GameObject* gameobject);

	btRigidBody* GetRigidBody();
	btCollisionShape* GetCollisionShape();
private:
	std::string name = "None";
	std::string tag = "None";
	btCollisionShape* collider;
	btTransform transform;
	btRigidBody* body;
};



namespace PhysicsManagerBullet{
	void Init();
	void Update(float deltaTime);
	void AddColliderShape(btCollisionShape* collider);
	void AddCollider(Collider collider);
	void RemoveCollider(std::string name);
	void AddRigidBody(btRigidBody* body);
	Collider* GetCollider(std::string Name);
}
