#pragma once
#include "Engine/Core/AssetManager.h"
#include <iostream>

class Scene
{
public:
	Scene();

	void Load();
	void Update(float dt);
	void RenderObjects();

private:
	AssetManager assetManager;

};

