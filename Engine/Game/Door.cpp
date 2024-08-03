#include "Door.h"

Door::Door(std::string Name, Model* door, Model* frame, glm::vec3 position) {
	name = Name;
	AssetManager::AddGameObject(GameObject(name + "_frame", frame, position, false, 0, Box, 0, 0, 0));
	AssetManager::AddGameObject(name + "_door", door, position, false, 0, Convex);
	GameObject* gameobject = AssetManager::GetGameObject(name + "_door");
	gameobject->GetRigidBody()->setCcdMotionThreshold(0.1);
	gameobject->GetRigidBody()->setCcdSweptSphereRadius(0.2); // Set the radius for CCD
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
