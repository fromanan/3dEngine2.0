#pragma once
#include "Engine/Core/AssetManager.h"
#include "Engine/Core/GameObject.h"
#include "Engine/Core/Renderer.h"
#include "Engine/Core/Camera.h"
#include "Engine/Core/Camera.h"
#include "Engine/Backend.h"
#include "Engine/Player.h"
#include <iostream>
#include <vector>
#include <ostream>

class Scene
{
public:
	Scene();

	void Load();
	void Update(float dt);
	void RenderObjects();

private:
	//this is the sun
	glm::vec3 lightPos;
	SkyBox sky;
};

