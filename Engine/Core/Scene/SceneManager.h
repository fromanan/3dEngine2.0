#pragma once
#include "Engine/Core/Scene/Scene.h"
#include <vector>
#include <iostream>

namespace SceneManager {

	void Init();
	int CreateScene(Scene scene);
	void LoadScene(int sceneNumber);
	void Update(float dt);
	void Render();
	Scene* GetCurrentScene();
};


