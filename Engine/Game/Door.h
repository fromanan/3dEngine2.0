#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

#include "Engine/Core/GameObject.h"
#include "Engine/Game/Player.h"
#include "Engine/Physics/Physics.h"


class Door
{
public:
	Door(const char* frame, Texture* frameTexture, const char* door, Texture* doorTexture, glm::vec3 position, std::string name);
	void Interact();
	void Update();
private:
	bool isOpened = false;
	bool Opening = false;
	float doorOpenSpeed = 0.025;
	GameObject* frame;
	GameObject* door;
	Cube* collider;
	std::string name;
};

