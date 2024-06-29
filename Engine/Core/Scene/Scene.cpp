#pragma once
#include "Scene.h"


Scene::Scene() {
	assetManager = AssetManager();
}


void Scene::Load() {
	assetManager.AddTexture("uvmap", "Assets/Textures/uvmap.DDS");
	assetManager.AddTexture("container", "Assets/Textures/Container.dds");
	assetManager.AddGameObject("container", "Assets/Objects/ContainerFinal.obj", assetManager.GetTexture("container"), glm::vec3(4, 0, 4));
	assetManager.AddGameObject("monkey", "Assets/Objects/suzanne.obj", assetManager.GetTexture("uvmap"), glm::vec3(0, 0, 0));

	//sets renderer
	Renderer::UseProgram(Renderer::GetProgramID("Texture"));
	MatrixID = glGetUniformLocation(Renderer::GetCurrentProgramID(), "MVP");
	ViewMatrixID = glGetUniformLocation(Renderer::GetCurrentProgramID(), "V");
	ModelMatrixID = glGetUniformLocation(Renderer::GetCurrentProgramID(), "M");
}

void Scene::Update(float deltaTime) {

}

void Scene::RenderObjects() {

	Renderer::UseProgram(Renderer::GetProgramID("Texture"));
	LightID = glGetUniformLocation(Renderer::GetCurrentProgramID(), "LightPosition_worldspace");
	glm::vec3 lightPos = glm::vec3(100, 100, 100);
	Renderer::setVec3(LightID, lightPos);

	glm::mat4 ProjectionMatrix = Camera::getProjectionMatrix();
	glm::mat4 ViewMatrix = Camera::getViewMatrix();
	

	for (int i = 0; i < assetManager.GetAllGameObjects().size(); i++) {
		glm::mat4 ModelMatrix = assetManager.GetAllGameObjects()[i].GetModelMatrix();
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		Renderer::setMat4(MatrixID,MVP);
		Renderer::setMat4(ModelMatrixID, ModelMatrix);
		Renderer::setMat4(ViewMatrixID, ViewMatrix);
		GLuint programid =  Renderer::GetCurrentProgramID();
		assetManager.GetAllGameObjects()[i].RenderObject(programid);
	}

	Renderer::UseProgram(Text2D::GetProgramID());
	std::ostringstream oss;
	oss << "Pos x:" << Camera::GetPostion().x << " y:" << Camera::GetPostion().y << " z:" << Camera::GetPostion().z;
	Renderer::RenderText(oss.str().c_str(), 0, 570, 15);

}