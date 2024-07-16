#pragma once
#include "Core/Input.h"
#include "Core/Camera.h"
#include "Physics/Physics.h"
#include "Engine/Game/Gun.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>


namespace Player
{
	extern RigidBody* rb;
	extern Cube* collider;
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
};

