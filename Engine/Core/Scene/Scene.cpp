#pragma once
#include "Scene.h"


Scene::Scene() {
	assetManager = AssetManager();
}
void Scene::Update(float deltaTime) {
}

void Scene::Load() {
	Texture multiColour = Texture("uvmap","Assets/Textures/uvmap.DDS");
	GameObject cube = GameObject("cube1", "Assets/Objects/suzanne.obj",&multiColour,glm::vec3(0,0,0));
	assetManager.AddGameObject(cube);

	GLuint LightID = glGetUniformLocation(Renderer::GetProgramID(), "LightPosition_worldspace");
	glm::vec3 lightPos = glm::vec3(100, 100, 100);
	GLuint MatrixID = glGetUniformLocation(Renderer::GetProgramID(), "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(Renderer::GetProgramID(), "V");
	GLuint ModelMatrixID = glGetUniformLocation(Renderer::GetProgramID(), "M");


}

void Scene::RenderObjects(GLuint programID) {
	std::vector<GameObject> gameObjects =  assetManager.GetAllGameObjects();

	for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i].RenderObject(programID);
	}
}