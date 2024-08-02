#include "PhysicsManager.h"
#include "Camera.h"
#include "Engine/Game/Player.h"

namespace PhysicsManager
{
	std::vector<Cube> colliders;
	std::vector<RigidBody> rigidbodies;

	// Forces
	constexpr float friction = 7;
	constexpr float gravity = -12;

	bool UpdatedCamera = false;

	void PhysicsManager::Update(float deltaTime) {
		UpdatedCamera = false;
		for (int i = 0; i < rigidbodies.size(); i++) {
			// Add friction so you're not sliding
			rigidbodies[i].SetForceX(rigidbodies[i].GetForce().x / (1 + (deltaTime * friction)));
			rigidbodies[i].SetForceZ(rigidbodies[i].GetForce().z / (1 + (deltaTime * friction)));
			rigidbodies[i].SetForceY(rigidbodies[i].GetForce().y + gravity * deltaTime);

			Cube* rb_collider = GetCollider(rigidbodies[i].GetCollider());
			
			// Do collider Calculation
			if (rb_collider != nullptr) {
				for (int col = 0; col < colliders.size(); col++) {
					if (colliders[i].ShouldDelete()) {
						colliders.erase(colliders.begin() + col);
						continue;
					}

					if (rb_collider->GetName() == colliders[col].GetName() || colliders[col].GetIsTrigger())
						continue;
					if (rigidbodies[i].GetForce().x < 0 && rb_collider->TouchingLeft(&colliders[col], rigidbodies[i].GetForce().x * deltaTime))
						rigidbodies[i].SetForce(-0.2f * rigidbodies[i].GetForce());
						///rigidbodies[i].RemoveForceX();
					if (rigidbodies[i].GetForce().x > 0 && rb_collider->TouchingRight(&colliders[col], rigidbodies[i].GetForce().x * deltaTime))
						rigidbodies[i].SetForceX(-0.2f * rigidbodies[i].GetForce().x);
						//rigidbodies[i].RemoveForceX();
					if (rigidbodies[i].GetForce().z > 0 && rb_collider->TouchingBack(&colliders[col], rigidbodies[i].GetForce().z * deltaTime))
						rigidbodies[i].SetForceZ(-0.2f * rigidbodies[i].GetForce().z);
						//rigidbodies[i].RemoveForceZ();
					if (rigidbodies[i].GetForce().z < 0 && rb_collider->TouchingFront(&colliders[col], rigidbodies[i].GetForce().z * deltaTime))
						rigidbodies[i].SetForceZ(-0.2f * rigidbodies[i].GetForce().z);
						//rigidbodies[i].RemoveForceZ();
					if (rigidbodies[i].GetForce().y > 0 && rb_collider->TouchingTop(&colliders[col], rigidbodies[i].GetForce().y * deltaTime))
						rigidbodies[i].SetForceY(-0.9f * rigidbodies[i].GetForce().y);
						//rigidbodies[i].RemoveForceY();
					if (rigidbodies[i].GetForce().y < 0 && rb_collider->TouchingBottom(&colliders[col], rigidbodies[i].GetForce().y * deltaTime))
						rigidbodies[i].SetForceY(-0.9f * rigidbodies[i].GetForce().y);
						//rigidbodies[i].RemoveForceY();
				}
				if (!UpdatedCamera)
					UpdatedCamera = true;
			}
			
			// Add velocity to position
			rigidbodies[i].NewPosition(deltaTime);
		}
	}
	
	RigidBody* PhysicsManager::AddRigidBody(const glm::vec3 position, const std::string& name) {
		rigidbodies.push_back(RigidBody(position, name));
		return &rigidbodies[rigidbodies.size() - 1];
	}
	
	Cube* PhysicsManager::AddCube(const glm::vec3 position, glm::vec3 min, glm::vec3 max, const std::string& name) {
		colliders.push_back(Cube(position, min, max, name));
		return &colliders[colliders.size() - 1];
	}
	
	Cube* PhysicsManager::AddCube(const glm::vec3 position, float width, float height, float depth, const std::string& name) {
		colliders.push_back(Cube(position, width, height,depth, name));
		return &colliders[colliders.size() - 1];
	}
	
	Cube* AddCube(GameObject* gameObject, const std::string& name) {
		colliders.push_back(Cube(gameObject, name));
		return nullptr;
	}

	Cube* PhysicsManager::GetCollider(std::string name) {
		for (int i = 0; i < colliders.size(); i++) {
			if (colliders[i].GetName() == name)
				return &colliders[i];
		}
		return nullptr;
	}
	
	RigidBody* PhysicsManager::GetRigidBody(std::string name) {
		for (int i = 0; i < rigidbodies.size(); i++) {
			if (rigidbodies[i].GetName() == name)
				return &rigidbodies[i];
		}
		return nullptr;
	}
	
	void PhysicsManager::RemoveCube(std::string name) {
		for (int i = 0; i < colliders.size(); i++) {
			if (colliders[i].GetName() == name)
				colliders.erase(colliders.begin() + i);
		}
	}
	
	void RemoveRigidBody(std::string name) {
		for (int i = 0; i < rigidbodies.size(); i++) {
			if (rigidbodies[i].GetName() == name)
				rigidbodies.erase(rigidbodies.begin() + i);
		}
	}
}
