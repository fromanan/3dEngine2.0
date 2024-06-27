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


class AssetManager
{
public:
	AssetManager();

	//returns index of object
	int AddGameObject(GameObject gameobject);
	int AddGameObject(const char* name, const char* path,Texture* texture, glm::vec3 position);

	int AddTexture(Texture texture);
	int AddTexture(const char* name, const char* path);

	void RemoveGameObject(const char* name);
	void RemoveGameObject(int index);

	GameObject* GetGameObject(const char* name);
	GameObject* GetGameObject(int index);

	Texture* GetTexture(const char* name);
private:
	std::vector<GameObject> GameObjects;
	std::vector<Texture> Textures;

};

