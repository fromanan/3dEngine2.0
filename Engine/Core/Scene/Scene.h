#pragma once
#include "Engine/Audio/Audio.h"
#include "Engine/Core/AssetManager.h"
#include "Engine/Core/GameObject.h"
#include "Engine/Core/Renderer.h"
#include "Engine/Core/Camera.h"
#include "Engine/Core/Camera.h"
#include "Engine/Backend.h"
#include "Engine/Game/Player.h"
#include "Engine/Game/Door.h"
#include "Engine/Game/Gun.h"
#include "Engine/Core/Animation.h"


#include <iostream>
#include <vector>
#include <ostream>
#include "Engine/Game/Door.h"
#include "Engine/Game/Gun.h"


class Scene
{
public:
	Scene();

	void Load();
	void Update(float dt);
	void RenderObjects();

	void AddGunPickUp(GunPickUp gunpickup);
	void AddGunPickUp(std::string gunName,std::string gunObject,glm::vec3 Position);

	int GetGunPickUpSize();

private:
	//this is the sun
	glm::vec3 lightPos;
	SkyBox sky;
	//objects
	std::vector<Door> doors;
	std::vector<GunPickUp> gunPickUps;
};

