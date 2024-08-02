#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include "Engine/Audio/Audio.h"
#include "Core/Input.h"
#include "Core/Camera.h"
#include "Engine/Game/Gun.h"
#include "Engine/Core/Common.h"

namespace Player
{
	extern GameObject* playerModel;
	extern GameObject* gun;
	glm::vec3 getPosition();
	glm::vec3 getForward();
	void setPosition(glm::vec3 pos);
	void Update(float deltaTime);
	void Init();
	std::string GetInteractingWithName();
	std::string getCurrentGun();

	bool SelectWeapon(std::string weaponName);
	bool OnGround();

	void SwitchWeapons(int index);
	void Shoot();
};

