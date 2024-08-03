#pragma once

#include "Scene.h"

namespace SceneManager
{
	void Init();
	size_t CreateScene(const Scene& scene);
	void LoadScene(int sceneNumber);
	void Update(float dt);
	void Render();
	Scene* GetCurrentScene();
}
