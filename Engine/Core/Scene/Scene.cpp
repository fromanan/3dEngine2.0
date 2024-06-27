#pragma once
#include "Scene.h"


Scene::Scene() {
	assetManager = AssetManager();
}
void Scene::Update(float deltaTime) {
}

void Scene::Load() {
	Texture multiColour = Texture("uvmap","Assets/Textures/uvmap.DDS");
	GameObject cube = GameObject("cube1", "Assets/Objects/suzanne.obj",&multiColour,glm::vec3(0,0,5));
	assetManager.AddGameObject(cube);
}

void Scene::RenderObjects(GLuint programID) {
	std::vector<GameObject> gameObjects =  assetManager.GetAllGameObjects();

	for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i].RenderObject(programID);
	}
}