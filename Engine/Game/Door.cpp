#include "Door.h"


Door::Door(std::string Name, const char* doorPath, const char* framePath, Texture* doorTexture, Texture* frameTexture, glm::vec3 position) {
	name = Name;

	AssetManager::AddGameObject(name+"_frame", framePath, frameTexture, position, false,0,Convex);
	AssetManager::AddGameObject(name+"_door", doorPath, doorTexture, position, false,0,Convex);

	//PhysicsManager::AddCube(AssetManager::GetGameObject(name + "_door"),name);
	//PhysicsManager::GetColider(name)->SetStatic(false);

	opened = false;
	opening = false;
	

	door_position = position;
}
void Door::Interact() {
	if (Player::GetInteractingWithName() == name)
	{
		opening = true;
		if(opened)
			AudioManager::PlaySound("door_close", door_position);
		else
			AudioManager::PlaySound("door_open", door_position);
	}
}
void Door::Update(float deltaTime) {
	if (opening) {
		if (rotaion >= 1.5f) {
			rotaion = 0;
			opening = false;
			opened = !opened;
			return;
		}
		if(!opened)
			AssetManager::GetGameObject(name + "_door")->SetRotationY(AssetManager::GetGameObject(name + "_door")->getRotation().y + openingSpeed);
		else
			AssetManager::GetGameObject(name + "_door")->SetRotationY(AssetManager::GetGameObject(name + "_door")->getRotation().y - openingSpeed);

		rotaion += openingSpeed;
		
	}
}