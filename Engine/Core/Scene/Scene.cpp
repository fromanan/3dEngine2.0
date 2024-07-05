#pragma once
#include "Scene.h"


Scene::Scene() {
	
}


void Scene::Load() {
	AssetManager::AddTexture("default", "Assets/Textures/default.dds");
	AssetManager::AddTexture("uvmap", "Assets/Textures/uvmap.DDS");
	AssetManager::AddTexture("container", "Assets/Textures/Container.dds");
	AssetManager::AddTexture("glock", "Assets/Textures/glock.dds");

	AssetManager::LoadAssets("Assets/Saves/mainScene.json");
	AssetManager::AddGameObject("floor", "Assets/Objects/Floor.obj", AssetManager::GetTexture("uvmap"), glm::vec3(0, 0, 0));

	//AssetManager::AddGameObject("cube3", "Assets/Objects/cube.obj", AssetManager::GetTexture("uvmap"), glm::vec3(-2, 0, -2));
	//AssetManager::AddGameObject("cube4", "Assets/Objects/cube.obj", AssetManager::GetTexture("uvmap"), glm::vec3(0, 1, 7));
	//AssetManager::AddGameObject("cube5", "Assets/Objects/cube.obj", AssetManager::GetTexture("uvmap"), glm::vec3(0, 2, 2));
	//AssetManager::GetGameObject("cube5")->SetParentName("cube4");




	PhysicsManager::AddCube(AssetManager::GetGameObject("floor"), "floor_collider");
	PhysicsManager::AddCube(AssetManager::GetGameObject("cube3"), "cube_collider");
	PhysicsManager::AddCube(AssetManager::GetGameObject("container"), "container_collider");



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
	sky = SkyBox(faces);

	Player::Init();
	Player::setPosition(glm::vec3(0, 5, 5));

	//AssetManager::SaveAssets("Assets/Saves/mainScene.json");
}

void Scene::Update(float deltaTime) {
	Player::Update(deltaTime);

}

void Scene::RenderObjects() {
	glm::mat4 ProjectionMatrix = Camera::getProjectionMatrix();
	glm::mat4 ViewMatrix = Camera::getViewMatrix();

	Renderer::RendererSkyBox(ViewMatrix, ProjectionMatrix, sky);
	Renderer::UseProgram(Renderer::GetProgramID("Texture"));
	Renderer::SetLightPos(lightPos);
	
	for (int i = 0; i < AssetManager::GetAllGameObjects().size(); i++) {
		glm::mat4 ModelMatrix = AssetManager::GetAllGameObjects()[i].GetModelMatrix();
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		Renderer::SetTextureShader(MVP, ModelMatrix, ViewMatrix);
		GLuint programid =  Renderer::GetCurrentProgramID();
		AssetManager::GetAllGameObjects()[i].RenderObject(programid);
	}

	std::ostringstream oss;
	oss << "Position x:" << Camera::GetPostion().x << " y:" << Camera::GetPostion().y << " z:" << Camera::GetPostion().z;
	Renderer::RenderText(oss.str().c_str(), 0, 570, 15);
	glm::vec3 vel = Player::rb->GetForce();
	oss.str(""); oss.clear();
	oss << "Velocity x:" << vel.x << " y:" << vel.y << " z:" << vel.z;
	Renderer::RenderText(oss.str().c_str(), 0, 540, 15);
	oss.str(""); oss.clear();
	oss << "Looking at: " << Camera::GetLookingAtName();
	Renderer::RenderText(oss.str().c_str(), 0, 520, 15);

}