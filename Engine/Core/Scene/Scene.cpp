#pragma once
#include "Scene.h"


Scene::Scene() {
	assetManager = AssetManager();
}


void Scene::Load() {
	assetManager.AddTexture("default", "Assets/Textures/default.dds");
	assetManager.AddTexture("uvmap", "Assets/Textures/uvmap.DDS");
	assetManager.AddTexture("container", "Assets/Textures/Container.dds");
	assetManager.LoadAssets("Assets/Saves/mainScene.json");
	assetManager.AddGameObject("cube3", "Assets/Objects/cube.obj", assetManager.GetTexture("uvmap"), glm::vec3(-2, 0, -2));
	PhysicsManager::AddCube(glm::vec3(0, 0.5, 0), 30, 1, 30, "floor_collider");
	PhysicsManager::AddCube(assetManager.GetGameObject("cube3"), "cube_collider");
	PhysicsManager::AddCube(assetManager.GetGameObject("container"), "container_collider");


	//sets renderer
	Renderer::UseProgram(Renderer::GetProgramID("Texture"));
	lightPos = glm::vec3(100, 100, 100);

	std::vector<std::string> faces{
		"Assets/Skybox/daylight/right.png",
			"Assets/Skybox/daylight/left.png",
			"Assets/Skybox/daylight/top.png",
			"Assets/Skybox/daylight/bottom.png",
			"Assets/Skybox/daylight/front.png",
			"Assets/Skybox/daylight/back.png"
	};
	space = SkyBox(faces);

	Player::Init();
	Player::setPosition(glm::vec3(0, 0, 5));

	//assetManager.SaveAssets("Assets/Saves/mainScene.json");
}

void Scene::Update(float deltaTime) {
	Player::Update(deltaTime);
}

void Scene::RenderObjects() {
	glm::mat4 ProjectionMatrix = Camera::getProjectionMatrix();
	glm::mat4 ViewMatrix = Camera::getViewMatrix();

	Renderer::RendererSkyBox(ViewMatrix, ProjectionMatrix, space);
	Renderer::UseProgram(Renderer::GetProgramID("Texture"));
	Renderer::SetLightPos(lightPos);
	
	for (int i = 0; i < assetManager.GetAllGameObjects().size(); i++) {
		glm::mat4 ModelMatrix = assetManager.GetAllGameObjects()[i].GetModelMatrix();
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		Renderer::SetTextureShader(MVP, ModelMatrix, ViewMatrix);
		GLuint programid =  Renderer::GetCurrentProgramID();
		assetManager.GetAllGameObjects()[i].RenderObject(programid);
	}

	std::ostringstream oss;
	oss << "Position x:" << Camera::GetPostion().x << " y:" << Camera::GetPostion().y << " z:" << Camera::GetPostion().z;
	Renderer::RenderText(oss.str().c_str(), 0, 570, 15);
	glm::vec3 vel = Player::rb->GetForce();
	oss.str(""); oss.clear();
	oss << "Velocity x:" << vel.x << " y:" << vel.y << " z:" << vel.z;
	Renderer::RenderText(oss.str().c_str(), 0, 540, 15);

}