#pragma once
#include "Engine/Core/AssetManager.h"
#include "Engine/Core/GameObject.h"
#include "Engine/Core/Renderer.h"
#include "Engine/Backend.h"
#include <iostream>
#include <vector>

class Scene
{
public:
	Scene();

	void Load();
	void Update(float dt);
	void RenderObjects(GLuint programID);

private:
	AssetManager assetManager;

};

