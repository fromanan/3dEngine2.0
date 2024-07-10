#pragma once
#include "Scene.h"


Scene::Scene() {
	
}


void Scene::Load() {
	AssetManager::AddTexture("uvmap", "Assets/Textures/uvmap.png");
	AssetManager::AddTexture("target", "Assets/Textures/target.jpeg");
	AssetManager::AddTexture("container", "Assets/Textures/Container.png");
	AssetManager::AddTexture("bullet_hole", "Assets/Textures/bullet_hole.png");

	AssetManager::LoadAssets("Assets/Saves/mainScene.json");
	AssetManager::AddGameObject("floor", "Assets/Objects/Floor.obj", AssetManager::GetTexture("uvmap"), glm::vec3(0, 0, 0));
	AssetManager::AddGameObject("floor", "Assets/Objects/capsule.obj", AssetManager::GetTexture("uvmap"), glm::vec3(0,0,5));

	//AssetManager::AddGameObject("target1", "Assets/Objects/ShootingTarget.obj", AssetManager::GetTexture("target"), glm::vec3(-4, 1.5, 0));
	//AssetManager::GetGameObject("target1")->SetScale(0.3);

	//AssetManager::AddGameObject("point", "Assets/Objects/cube.obj", AssetManager::GetTexture("uvmap"), glm::vec3(0, 0, 0));
	//AssetManager::GetGameObject("point")->SetScale(0.05);
	//AssetManager::AddGameObject("cube3", "Assets/Objects/cube.obj", AssetManager::GetTexture("uvmap"), glm::vec3(-2, 0, -2));
	//AssetManager::AddGameObject("cube4", "Assets/Objects/cube.obj", AssetManager::GetTexture("uvmap"), glm::vec3(0, 1, 7));
	//AssetManager::AddGameObject("cube5", "Assets/Objects/cube.obj", AssetManager::GetTexture("uvmap"), glm::vec3(0, 2, 2));
	//AssetManager::GetGameObject("cube5")->SetParentName("cube4");

	//PhysicsManager::AddCube(AssetManager::GetGameObject("target1"), "target1_collider");
	PhysicsManager::AddCube(AssetManager::GetGameObject("floor"), "floor_collider");
	PhysicsManager::AddCube(AssetManager::GetGameObject("cube3"), "cube_collider");
	PhysicsManager::AddCube(AssetManager::GetGameObject("container"), "container_collider");

	doors.push_back(Door("door1", "Assets/Objects/door.obj", "Assets/Objects/door_frame.obj", AssetManager::GetTexture("uvmap"), AssetManager::GetTexture("uvmap"), glm::vec3(-5, 0, -5)));

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

	WeaponManager::Init();

	Player::Init();
	Player::setPosition(glm::vec3(0, 5, 5));

	//AssetManager::SaveAssets("Assets/Saves/mainScene.json");
}

void Scene::Update(float deltaTime) {
	Player::Update(deltaTime);

	for (int door = 0; door < doors.size(); door++) {
		doors[door].Interact();
		doors[door].Update(deltaTime);
	}
}

void Scene::RenderObjects() {
	glm::mat4 ProjectionMatrix = Camera::getProjectionMatrix();
	glm::mat4 ViewMatrix = Camera::getViewMatrix();

	Renderer::RendererSkyBox(ViewMatrix, ProjectionMatrix, sky);
	Renderer::UseProgram(Renderer::GetProgramID("Texture"));
	Renderer::SetLightPos(lightPos);
	
	GLuint programid = Renderer::GetCurrentProgramID();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < AssetManager::GetAllGameObjects().size(); i++) {
		glm::mat4 ModelMatrix = AssetManager::GetGameObject(i)->GetModelMatrix();
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		Renderer::SetTextureShader(MVP, ModelMatrix, ViewMatrix);
		AssetManager::GetGameObject(i)->RenderObject(programid);
	}
	for (int i = 0; i < AssetManager::GetAllDecals()->size(); i++)
	{
		glm::mat4 ModelMatrix = AssetManager::GetDecal(i)->GetModel();
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		Renderer::SetTextureShader(MVP, ModelMatrix, ViewMatrix);
		AssetManager::GetDecal(i)->RenderDecal(programid);
	}
	glDisable(GL_BLEND);

	std::ostringstream oss;
	oss.precision(2);
	oss << "Pos x:" << Camera::GetPostion().x << " y:" << Camera::GetPostion().y << " z:" << Camera::GetPostion().z;
	Renderer::RenderText(oss.str().c_str(), 0, 570, 15);
	glm::vec3 vel = Player::rb->GetForce();
	oss.str(""); oss.clear();
	oss.precision(2);
	oss << "Vel x:" << vel.x << " y:" << vel.y << " z:" << vel.z;
	Renderer::RenderText(oss.str().c_str(), 0, 540, 15);
	oss.str(""); oss.clear();
	oss << Player::getCurrentGun()->currentammo << "/" << Player::getCurrentGun()->ammo;
	Renderer::RenderText(oss.str().c_str(), 660, 0, 15);

}

