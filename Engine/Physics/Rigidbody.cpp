#include "RigidBody.h"

#include "PhysicsManager.h"

RigidBody::RigidBody() = default;

RigidBody::RigidBody(const glm::vec3 position, const std::string& name) {
	this->position = position;
	this->name = name;
}

glm::vec3 RigidBody::GetPosition() const {
	return position;
}

void RigidBody::SetPosition(glm::vec3 position) {
	this->position = position;
}

std::string RigidBody::GetName() {
	return name;
}

void RigidBody::NewPosition(float deltaTime) {
	position += velocity * deltaTime;
	if (collider != "None")
		PhysicsManager::GetCollider(collider)->setPosition(position);
}

void RigidBody::NewPositionY(float deltaTime) {
	position.y += velocity.y * deltaTime;
}

void RigidBody::AddForce(const glm::vec3 force) {
	velocity += force;
}

void RigidBody::AddForceX(float force) {
	velocity.x += force;
}

void RigidBody::AddForceY(float force) {
	velocity.y += force;
}

void RigidBody::AddForceZ(float force) {
	velocity.z += force;
}

void RigidBody::SetForce(const glm::vec3 force) {
	velocity = force;
}

void RigidBody::SetForceX(float force) {
	velocity.x = force;
}

void RigidBody::SetForceY(float force) {
	velocity.y = force;
}

void RigidBody::SetForceZ(float force) {
	velocity.z = force;
}

void RigidBody::RemoveForceX() {
	velocity.x = 0;
}

void RigidBody::RemoveForceY() {
	velocity.y = 0;
}

void RigidBody::RemoveForceZ() {
	velocity.z = 0;
}

void RigidBody::SetCollider(const std::string& colliderName) {
	this->collider = colliderName;
}

std::string RigidBody::GetCollider() {
	return collider;
}

glm::vec3 RigidBody::GetForce() {
	return { 0, 0, 0 };
}
