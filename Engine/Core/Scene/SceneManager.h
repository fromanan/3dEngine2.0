#pragma once
#include <vector>
#include <iostream>

#include "Engine/Core/Scene/Scene.h"

namespace SceneManager
{
	void Init();
	size_t CreateScene(Scene scene);
	void LoadScene(int sceneNumber);
	void Update(float dt);
	void Render();
	Scene* GetCurrentScene();
}
