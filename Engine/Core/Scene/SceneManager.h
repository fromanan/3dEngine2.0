#pragma once
#include "Scene.h"
#include <vector>
#include <iostream>

class SceneManager
{
public:
	SceneManager();
	int CreateScene(Scene scene);
	void LoadScene(int sceneNumber);
	void Update(float dt);
	void Render();

private:
	std::vector<Scene> scenes;
	int currentScene;
};

