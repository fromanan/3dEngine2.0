#include "pch.h"

#include "Crate.h"

#include "AssetManager.h"
#include "Texture.h"

Crate::Crate(const glm::vec3 position, const std::string& name, const char* path, Texture* texture) {
	this->name = name;
	AssetManager::AddGameObject(name, path, texture, position, false,3,Convex);
	AssetManager::GetGameObject(name)->GetRigidBody()->setCcdMotionThreshold(0.1f);
	AssetManager::GetGameObject(name)->GetRigidBody()->setCcdSweptSphereRadius(0.2f); // Set the radius for CCD
}

void Crate::Update() const {
	if (health <= 0) {
		Break();
	}
}

std::string Crate::GetName() {
	return name;
}

void Crate::DealDamage(const int damage, glm::vec3 position, glm::vec3 force) {
	health -= damage;
}

void Crate::Break() const {
	AssetManager::RemoveGameObject(name);
}
