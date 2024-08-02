#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <fstream>
#include <iostream>

#include "GameObject.h"
#include "Common.h"
#include "Renderer.h"
#include "Decal.h"

// Json stuff
#include "Loaders/nlohmann/json.hpp"
using json = nlohmann::json;

namespace AssetManager
{
	void Init();
	void SaveAssets(const char* path);
	void LoadAssets(const char* path);

	// returns index of object
	size_t AddGameObject(const GameObject& gameObject);
	size_t AddGameObject(const std::string& name, const char* path, Texture* texture, glm::vec3 position,
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
