#include "Door.h"

Door::Door(std::string Name, const char* doorPath, const char* framePath, Texture* doorTexture, Texture* frameTexture, glm::vec3 position) {
	name = Name;
	AssetManager::AddGameObject(GameObject(name + "_frame", framePath, frameTexture, position, false, 0, Box, 0, 0, 0));
	AssetManager::AddGameObject(name + "_door", doorPath, doorTexture, position, false, 0, Convex);
	GameObject* gameObject = AssetManager::GetGameObject(name + "_door");
	gameObject->GetRigidBody()->setCcdMotionThreshold(0.1f);
	gameObject->GetRigidBody()->setCcdSweptSphereRadius(0.2f); // Set the radius for CCD
	opened = false;
	opening = false;
	door_position = position;
}

void Door::Interact() {
	if (Player::GetInteractingWithName() == name + "_door") {
		opening = true;
		if (opened)
			AudioManager::PlaySound("door_close", door_position);
		else
			AudioManager::PlaySound("door_open", door_position);
	}
}

void Door::Update(float deltaTime) {
	if (!opening)
		return;
	
	if (rotation >= 1.5f) {
		rotation = 0;
		opening = false;
		opened = !opened;
		return;
	}
		
	if (!opened)
		AssetManager::GetGameObject(name + "_door")->SetRotationY(AssetManager::GetGameObject(name + "_door")->getRotation().y + openingSpeed);
	else
		AssetManager::GetGameObject(name + "_door")->SetRotationY(AssetManager::GetGameObject(name + "_door")->getRotation().y - openingSpeed);

	rotation += openingSpeed;
}
