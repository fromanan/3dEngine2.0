#pragma once
#include "AssetManager.h"

AssetManager::AssetManager() {
	Textures.clear();
	GameObjects.clear();

}

//returns index of object
int AssetManager::AddGameObject(GameObject gameobject) {
	GameObjects.push_back(gameobject);
	return GameObjects.size() - 1;
}
int AssetManager::AddGameObject(const char* name, const char* path, Texture* texture, glm::vec3 position) {
	GameObject temp(name, path, texture, position);
	GameObjects.push_back(temp);
	return GameObjects.size() - 1;
}

int AssetManager::AddTexture(Texture texture) {
	Textures.push_back(texture);
	return Textures.size() - 1;
}
int AssetManager::AddTexture(const char* name, const char* path) {
	Texture temp = Texture(name, path);
	Textures.push_back(temp);
	return Textures.size() - 1;
}

void AssetManager::RemoveGameObject(const char* name) {
	for (int i = 0; i < GameObjects.size(); i++) {
		if (GameObjects[i].GetName() == name)
			GameObjects.erase(GameObjects.begin() + i);
	}
}
void AssetManager::RemoveGameObject(int index) {
	GameObjects.erase(GameObjects.begin() + index);
}

GameObject* AssetManager::GetGameObject(const char* name) {
	for (int i = 0; i < GameObjects.size(); i++) {
		if (GameObjects[i].GetName() == name)
			return &GameObjects[i];
	}
}
GameObject* AssetManager::GetGameObject(int index) {
	return &GameObjects[index];

}
std::vector<GameObject> AssetManager::GetAllGameObjects() {
	return GameObjects;
}

Texture* AssetManager::GetTexture(const char* name) {
	for (int i = 0; i < Textures.size(); i++) {
		if (Textures[i].GetName() == name)
			return &Textures[i];
	}
}