#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "GameObject.h"
#include "Common.h"
#include "Renderer.h"

//Json stuff
#include <fstream>
#include <iostream>
#include "Loaders/nlohmann/json.hpp"
using json = nlohmann::json;


class AssetManager
{
public:
	AssetManager();
	void SaveAssets(const char* path);
	void LoadAssets(const char* path);

	//returns index of object
	int AddGameObject(GameObject gameobject);
	int AddGameObject(std::string name, const char* path,Texture* texture, glm::vec3 position);

	int AddTexture(Texture texture);
	int AddTexture(const char* name, const char* path);

	void RemoveGameObject(const char* name);
	void RemoveGameObject(int index);

	GameObject* GetGameObject(const char* name);
	GameObject* GetGameObject(int index);
	std::vector<GameObject> GetAllGameObjects();

	Texture* GetTexture(const char* name);

private:
	std::vector<GameObject> GameObjects;
	std::vector<Texture> Textures;

};

