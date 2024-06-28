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


	

	MatrixID = glGetUniformLocation(Renderer::GetProgramID(), "MVP");
	ViewMatrixID = glGetUniformLocation(Renderer::GetProgramID(), "V");
	ModelMatrixID = glGetUniformLocation(Renderer::GetProgramID(), "M");


}

void Scene::RenderObjects(GLuint programID) {

	Renderer::UseProgram(Renderer::GetProgramID());

	LightID = glGetUniformLocation(Renderer::GetProgramID(), "LightPosition_worldspace");
	glm::vec3 lightPos = glm::vec3(100, 100, 100);
	Renderer::setVec3(LightID, lightPos);

	glm::mat4 ProjectionMatrix = Camera::getProjectionMatrix();
	glm::mat4 ViewMatrix = Camera::getViewMatrix();

	std::vector<GameObject> gameObjects =  assetManager.GetAllGameObjects();
	

	for (int i = 0; i < gameObjects.size(); i++) {

		glm::mat4 ModelMatrix = gameObjects[i].GetModelMatrix();
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		Renderer::setMat4(MatrixID,MVP);
		Renderer::setMat4(ModelMatrixID, ModelMatrix);
		Renderer::setMat4(ViewMatrixID, ViewMatrix);

		gameObjects[i].RenderObject(programID);
	}
}