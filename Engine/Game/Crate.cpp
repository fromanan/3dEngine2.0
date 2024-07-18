#include "Crate.h"


Crate::Crate(glm::vec3 position, std::string name, const char* path, Texture* texture) {
	this->name = name;
	this->position = position;
	AssetManager::AddGameObject(name, path, texture, position, false);
	PhysicsManager::AddCube(AssetManager::GetGameObject(name), name);
	PhysicsManager::GetColider(name)->SetStatic(false);
}
void Crate::Update() {
	if (health <= 0) {
		Break();
	}
}
std::string Crate::GetName() {
	return name;
}	
void Crate::DealDamage(int damage) {
	health -= damage;
}
void Crate::Break() {
	AssetManager::RemoveGameObject(name);
	PhysicsManager::RemoveCube(name);
}