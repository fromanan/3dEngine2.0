#include "Door.h"
#include "Engine/Core/AssetManager.h"



Door::Door(const char* frame, Texture* frameTexture, const char* door, Texture* doorTexture, glm::vec3 position, std::string name) {
	this->name = name;
	this->frame = AssetManager::GetGameObject(AssetManager::AddGameObject(std::string(frame), frame, frameTexture, position));

	this->door = AssetManager::GetGameObject(AssetManager::AddGameObject(std::string(door), door, doorTexture, position));
	this->door->SetParentName(name);

	collider = PhysicsManager::AddCube(glm::vec3(position.x + 1, position.y + 2, position.z),2.5,2,0.5,name);
	collider->SetStatic(false);

}
void Door::Interact() {
	if (Player::GetInteractingWithName() == this->name && !Opening)
		Opening = true;
	else
		std::cout << Player::GetInteractingWithName() << std::endl;
	
}

void Door::Update() {
	if (isOpened)
		collider->SetIsTrigger(true);
	else
		collider->SetIsTrigger(false);

	if (Opening)
	{
		if (!isOpened)
			door->SetRotationY(door->getRotation().y + 0.05);
		else 
			door->SetRotationY(door->getRotation().y - 0.05);

		if (door->getRotation().y >= 1.5708 || door->getRotation().y <= 0)
		{
			Opening = false;
			isOpened = !isOpened;
		}
	}
}