#pragma once
#include "Decal.h"

namespace AssetManager
{
	void Init();
	void SaveAssets(const char* path);
	void LoadAssets(const char* path);

	// returns index of object
	size_t AddGameObject(const GameObject& gameObject);
	size_t AddGameObject(const std::string& name, Model* model, glm::vec3 position,
										bool save, float mass, ColliderShape shape);

	size_t AddDecal(glm::vec3 position, glm::vec3 normal, glm::vec3 scale, Texture* texture,
	                            GameObject* parent);
	Decal* GetDecal(int index);
	std::vector<Decal>* GetAllDecals();

	size_t AddTexture(Texture texture);
	size_t AddTexture(const char* name, const char* path);
	size_t AddTexture(const char* name, const char* path, const char* normalPath);

	void RemoveGameObject(const std::string& name);
	void RemoveGameObject(int index);

	void CleanUp();

	GameObject* GetGameObject(const std::string& name);
	GameObject* GetGameObject(int index);
	std::vector<GameObject> GetAllGameObjects();
	size_t GetGameObjectsSize();
	size_t GetDecalsSize();

	Texture* GetTexture(const std::string& name);
}
