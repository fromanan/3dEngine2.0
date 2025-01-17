#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Engine/Core/GameObject.h"

#include "Engine/Physics/BulletPhysics.h"
#include "Engine/Core/AssetManager.h"

#include <string>
#include <vector>

class Crate
{
public:
	Crate(glm::vec3 position, std::string name,const char* path, Texture* texture);
	void Update();
	void Break();
	void DealDamage(int damage, glm::vec3 position, glm::vec3 force);
	std::string GetName();

private:
	std::string name = "None";
	int health = 500;
};

