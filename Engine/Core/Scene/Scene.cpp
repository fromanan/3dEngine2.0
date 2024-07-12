#pragma once
#include "Scene.h"


Scene::Scene() {
	
}

void Scene::Load() {
	AssetManager::AddTexture("uvmap", "Assets/Textures/uvmap.png");
	AssetManager::AddTexture("target", "Assets/Textures/target.jpeg");
	AssetManager::AddTexture("container", "Assets/Textures/Container.png", "Assets/Normals/container_normal.png");
	AssetManager::AddTexture("bullet_hole", "Assets/Textures/bullet_hole.png");
	AssetManager::AddTexture("sand", "Assets/Textures/sandyGround.png","Assets/Normals/sand_normal1.png");
	AssetManager::AddTexture("concrete", "Assets/Textures/fence.png","Assets/Normals/fence_normal.tga");
	AssetManager::AddTexture("ak47_lowpoly", "Assets/Textures/ak47_lowpoly.png", "Assets/Normals/ak47_lowpoly_normal.png");


	//AssetManager::LoadAssets("Assets/Saves/mainScene.json");
	WeaponManager::Init();


	//AssetManager::AddGameObject("enemy1", "Assets/Objects/Enemy.obj", AssetManager::GetTexture("uvmap"), glm::vec3(0, 0, 0), true);
	AssetManager::AddGameObject("fence1", "Assets/Objects/fence1.obj", AssetManager::GetTexture("concrete"), glm::vec3(0, 0, 0), true);
	AssetManager::AddGameObject("fence2", "Assets/Objects/fence2.obj", AssetManager::GetTexture("concrete"), glm::vec3(0, 0, 0), true);
	AssetManager::AddGameObject("fence3", "Assets/Objects/fence3.obj", AssetManager::GetTexture("concrete"), glm::vec3(0, 0, 0), true);
	AssetManager::AddGameObject("fence4", "Assets/Objects/fence4.obj", AssetManager::GetTexture("concrete"), glm::vec3(0, 0, 0), true);
	AssetManager::AddGameObject("floor", "Assets/Objects/Floor.obj", AssetManager::GetTexture("sand"), glm::vec3(3, 0, 0), true);

	//AssetManager::AddGameObject("container", "Assets/Objects/container.obj", AssetManager::GetTexture("container"), glm::vec3(20, 2, 0), true);



	PhysicsManager::AddCube(AssetManager::GetGameObject("floor"), "floor_collider");
	PhysicsManager::AddCube(AssetManager::GetGameObject("fence1"), "fence1_collider");
	PhysicsManager::AddCube(AssetManager::GetGameObject("fence2"), "fence2_collider");
	PhysicsManager::AddCube(AssetManager::GetGameObject("fence3"), "fence3_collider");
	PhysicsManager::AddCube(AssetManager::GetGameObject("fence4"), "fence4_collider");

	//PhysicsManager::AddCube(AssetManager::GetGameObject("container"), "container_collider");

	PhysicsManager::AddCube(AssetManager::GetGameObject("floor"), "floor_collider");
	//PhysicsManager::AddCube(AssetManager::GetGameObject("cube3"), "cube_collider");
	//PhysicsManager::AddCube(AssetManager::GetGameObject("container"), "container_collider");

	doors.push_back(Door("door1", "Assets/Objects/door1.obj", "Assets/Objects/door_frame1.obj", AssetManager::GetTexture("uvmap"), AssetManager::GetTexture("uvmap"), glm::vec3(7, 0, 3)));

	gunPickUps.push_back(GunPickUp("ak47", "ak47_pickup", "Assets/Objects/ak47_lowpoly.obj", AssetManager::GetTexture("ak47_lowpoly"), glm::vec3(8, 1, -5)));
	gunPickUps.push_back(GunPickUp("pistol", "pistol_pickup", "Assets/Objects/glock.obj", AssetManager::GetTexture("uvmap"), glm::vec3(8, 1, -6)));


	//sets renderer
	Renderer::UseProgram(Renderer::GetProgramID("Texture"));
	lightPos = glm::vec3(0, 100, 20);

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
	Player::setPosition(glm::vec3(3, 5, 0));
	//AssetManager::SaveAssets("Assets/Saves/mainScene.json");

}

void Scene::Update(float deltaTime) {
	Player::Update(deltaTime);

	for (int door = 0; door < doors.size(); door++) {
		doors[door].Interact();
		doors[door].Update(deltaTime);
	}
	for (int gun = 0; gun < gunPickUps.size(); gun++) {
		if (gunPickUps[gun].Interact())
			gunPickUps.erase(gunPickUps.begin() + gun);
	}
}

void Scene::RenderObjects() {
	glm::mat4 ProjectionMatrix = Camera::getProjectionMatrix();
	glm::mat4 ViewMatrix = Camera::getViewMatrix();
	glm::mat4 PV = ProjectionMatrix * ViewMatrix;

	Renderer::RendererSkyBox(ViewMatrix, ProjectionMatrix, sky);
	Renderer::UseProgram(Renderer::GetProgramID("Texture"));
	Renderer::SetLightPos(lightPos);
	
	GLuint programid = Renderer::GetCurrentProgramID();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glm::mat3 ModelView3x3Matrix = glm::mat3(0, 0, 0, 0, 0, 0, 0, 0, 0);



	for (int i = 0; i < AssetManager::GetAllGameObjects().size(); i++) {
		GameObject* gameobjectRender = AssetManager::GetGameObject(i);
		glm::mat4 ModelMatrix = AssetManager::GetGameObject(i)->GetModelMatrix();
		glm::mat4 MVP = PV * ModelMatrix;
		//glm::mat3 ModelView3x3Matrix = glm::mat3(ViewMatrix * ModelMatrix); // Take the upper-left part of ModelViewMatrix

		Renderer::SetTextureShader(MVP, ModelMatrix, ViewMatrix, ModelView3x3Matrix);
		AssetManager::GetGameObject(i)->RenderObject(programid);
	}
	for (int i = 0; i < AssetManager::GetAllDecals()->size(); i++)
	{
		glm::mat4 ModelMatrix = AssetManager::GetDecal(i)->GetModel();
		glm::mat4 MVP = PV * ModelMatrix;
		//glm::mat3 ModelView3x3Matrix = glm::mat3(ViewMatrix * ModelMatrix); // Take the upper-left part of ModelViewMatrix
		Renderer::SetTextureShader(MVP, ModelMatrix, ViewMatrix, ModelView3x3Matrix);
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
	oss << WeaponManager::GetGunByName(Player::getCurrentGun())->currentammo << "/" << WeaponManager::GetGunByName(Player::getCurrentGun())->ammo;
	Renderer::RenderText(oss.str().c_str(), 660, 0, 15);
}

