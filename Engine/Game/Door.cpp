#include "Door.h"


Door::Door(std::string Name, const char* doorPath, const char* framePath, Texture* doorTexture, Texture* frameTexture, glm::vec3 position) {
	name = Name;

	AssetManager::AddGameObject(name+"_frame", framePath, frameTexture, position);
	AssetManager::AddGameObject(name+"_door", doorPath, doorTexture, position);

	PhysicsManager::AddCube(AssetManager::GetGameObject(name + "_door"),name);
	PhysicsManager::GetColider(name)->SetStatic(false);

	opened = false;
	opening = false;
}
void Door::Interact() {
	if (Player::GetInteractingWithName() == name)
		opening = true;
}
void Door::Update(float deltaTime) {
	if (opening) {
		if (rotaion >= 1.5f) {
			rotaion = 0;
			opening = false;
			opened = !opened;
			PhysicsManager::GetColider(name)->Regenerate(AssetManager::GetGameObject(name + "_door"));
			return;
		}
		if(!opened)
			AssetManager::GetGameObject(name + "_door")->SetRotationY(AssetManager::GetGameObject(name + "_door")->getRotation().y + openingSpeed);
		else
			AssetManager::GetGameObject(name + "_door")->SetRotationY(AssetManager::GetGameObject(name + "_door")->getRotation().y - openingSpeed);

		rotaion += openingSpeed;
		
	}
}