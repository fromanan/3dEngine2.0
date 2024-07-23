#pragma once
#include "Scene.h"

Scene::Scene() {
	
}

void Scene::Load() {
	AssetManager::AddTexture("uvmap", "Assets/Textures/uvmap.png");
	AssetManager::AddTexture("crate", "Assets/Textures/crate.png");

	AssetManager::AddTexture("target", "Assets/Textures/target.jpeg");
	AssetManager::AddTexture("container", "Assets/Textures/Container.png", "Assets/Normals/container_normal.png");
	AssetManager::AddTexture("bullet_hole", "Assets/Textures/bullet_hole.png");
	AssetManager::AddTexture("bullet_hole_glass", "Assets/Textures/bullet_hole_glass.png");

	AssetManager::AddTexture("sand", "Assets/Textures/sandyGround.png","Assets/Normals/sand_normal.png");
	AssetManager::AddTexture("concrete", "Assets/Textures/fence.png","Assets/Normals/fence_normal.png");
	AssetManager::AddTexture("ak47_lowpoly", "Assets/Textures/ak47_lowpoly.png", "Assets/Normals/ak47_lowpoly_normal.png");
	AssetManager::AddTexture("crosshair", "Assets/Sprites/CrossHair.png	", "Assets/Normals/ak47_lowpoly_normal.png");
	AssetManager::AddTexture("window", "Assets/Textures/window.png");


	//AssetManager::LoadAssets("Assets/Saves/mainScene.json");
	WeaponManager::Init();

	//AssetManager::AddGameObject("enemy1", "Assets/Objects/Enemy.obj", AssetManager::GetTexture("uvmap"), glm::vec3(0, 0, 0), true);
	//AssetManager::AddGameObject("fence1", "Assets/Objects/fence1.obj", AssetManager::GetTexture("concrete"), glm::vec3(0, 0, 0), true, 0, Box);
	//AssetManager::AddGameObject("fence2", "Assets/Objects/fence2.obj", AssetManager::GetTexture("concrete"), glm::vec3(0, 0, 0), true, 0, Box);
	//AssetManager::AddGameObject("fence3", "Assets/Objects/fence3.obj", AssetManager::GetTexture("concrete"), glm::vec3(0, 0, 0), true, 0, Box);
	//AssetManager::AddGameObject("fence4", "Assets/Objects/fence4.obj", AssetManager::GetTexture("concrete"), glm::vec3(0, 0, 0), true, 0, Box);
	AssetManager::AddGameObject("floor", "Assets/Objects/test_platform.obj", AssetManager::GetTexture("sand"), glm::vec3(0, -2, 0), true, 0, Box);

	//PhysicsManagerBullet::AddCollider(Collider(AssetManager::GetGameObject("floor"), "floor",Box));
	//PhysicsManagerBullet::AddCollider(Collider(AssetManager::GetGameObject("fence1"), "fence1_collider", Box));
	//PhysicsManagerBullet::AddCollider(Collider(AssetManager::GetGameObject("fence2"), "fence2_collider", Box));
	//PhysicsManagerBullet::AddCollider(Collider(AssetManager::GetGameObject("fence3"), "fence3_collider", Box));
	//PhysicsManagerBullet::AddCollider(Collider(AssetManager::GetGameObject("fence4"), "fence4_collider", Box));

	//PhysicsManager::AddCube(AssetManager::GetGameObject("floor"), "floor_collider");


	//doors.push_back(Door("door1", "Assets/Objects/door1.obj", "Assets/Objects/door_frame1.obj", AssetManager::GetTexture("uvmap"), AssetManager::GetTexture("uvmap"), glm::vec3(7, 0, 3)));

	//crates.push_back(Crate(glm::vec3(15, 10, 3), "crate1", "Assets/Objects/Crate.obj", AssetManager::GetTexture("crate")));
	crates.push_back(Crate(glm::vec3(1, 25, 1), "crate2", "Assets/Objects/Crate.obj", AssetManager::GetTexture("crate")));
	crates.push_back(Crate(glm::vec3(1, 30, 0.5), "crate2", "Assets/Objects/Crate.obj", AssetManager::GetTexture("crate")));
	crates.push_back(Crate(glm::vec3(0.5, 20, 1), "crate2", "Assets/Objects/Crate.obj", AssetManager::GetTexture("crate")));


	//windows.push_back(Window("window1", "Assets/Objects/window_frame1.obj", AssetManager::GetTexture("uvmap"), "Assets/Objects/window1.obj", AssetManager::GetTexture("window"), glm::vec3(9, 0.5, 5), glm::vec3(0, 3.14159265358979323846 / 2.0f, 0)));
	//windows.push_back(Window("window2", "Assets/Objects/window_frame1.obj", AssetManager::GetTexture("uvmap"), "Assets/Objects/window1.obj", AssetManager::GetTexture("window"), glm::vec3(9, 0.5, -5), glm::vec3(0, 3.14159265358979323846 / 2.0f, 0)));

	//gunPickUps.push_back(GunPickUp("ak47", "ak47_pickup1", "Assets/Objects/ak47_lowpoly.obj", AssetManager::GetTexture("ak47_lowpoly"), glm::vec3(8, 1, -5)));
	//gunPickUps.push_back(GunPickUp("glock", "glock_pickup1", "Assets/Objects/glock.obj", AssetManager::GetTexture("uvmap"), glm::vec3(8, 1, -6)));

	//gunPickUps.push_back(GunPickUp("ak47", "ak47_pickup", "Assets/Objects/ak47_lowpoly.obj", AssetManager::GetTexture("ak47_lowpoly"), glm::vec3(8, -12, -5)));
	//AssetManager::GetGameObject("ak47_pickup")->SetRender(false);
	//gunPickUps.push_back(GunPickUp("glock", "glock_pickup", "Assets/Objects/glock.obj", AssetManager::GetTexture("uvmap"), glm::vec3(8, -13, -6)));
	//AssetManager::GetGameObject("glock_pickup")->SetRender(false);

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
	Player::setPosition(glm::vec3(3, 10, 0));

	//AssetManager::SaveAssets("Assets/Saves/mainScene.json");
}

void Scene::Update(float deltaTime) {
	Player::Update(deltaTime);

	for (int i = 0; i < AssetManager::GetGameObjectsSize(); i++) {

		AssetManager::GetGameObject(i)->Update();
	}
	for (int door = 0; door < doors.size(); door++) {
		doors[door].Interact();
		doors[door].Update(deltaTime);
	}
	for (int gun = 0; gun < gunPickUps.size(); gun++) {
		gunPickUps[gun].Update();
		if (gunPickUps[gun].Interact() && Player::getCurrentGun() == "nothing")
			gunPickUps.erase(gunPickUps.begin() + gun);
	}
	for (int crate = 0; crate < crates.size(); crate++) {
		crates[crate].Update();
	}

	
	AudioManager::UpdateListener(Player::getPosition(),Player::getForward(),PhysicsManager::GetRigidbody("PlayerRB")->GetForce());
	AudioManager::Update();
}

void Scene::RenderObjects() {
	glm::mat4 ProjectionMatrix = Camera::getProjectionMatrix();
	glm::mat4 ViewMatrix = Camera::getViewMatrix();
	glm::mat4 PV = ProjectionMatrix * ViewMatrix;

	Renderer::RendererSkyBox(ViewMatrix, ProjectionMatrix, sky);
	Renderer::UseProgram(Renderer::GetProgramID("Texture"));
	
	GLuint programid = Renderer::GetCurrentProgramID();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Renderer::SetLightPos(lightPos);

	for (int i = 0; i < AssetManager::GetGameObjectsSize(); i++) {

		GameObject* gameobjectRender = AssetManager::GetGameObject(i);

		if (!gameobjectRender->ShouldRender())
			continue;

		glm::mat4 ModelMatrix = gameobjectRender->GetModelMatrix();
		glm::mat4 MVP = PV * ModelMatrix;
		glm::mat3 ModelView3x3Matrix = glm::mat3(ViewMatrix * ModelMatrix); // Take the upper-left part of ModelViewMatrix

		Renderer::SetTextureShader(MVP, ModelMatrix, ViewMatrix, ModelView3x3Matrix);
		gameobjectRender->RenderObject(programid);
	}
	for (int i = 0; i < windows.size(); i++)
	{
		windows[i].Render(Renderer::GetProgramID("Texture"), ViewMatrix, ProjectionMatrix);
	}

	for (int i = 0; i < AssetManager::GetDecalsSize(); i++)
	{
		Decal* decal = AssetManager::GetDecal(i);
		//do some pre normal calcualtions
		glm::mat4 ModelMatrix = decal->GetModel();
		glm::mat4 MVP = PV * ModelMatrix;
		glm::mat3 ModelView3x3Matrix = glm::mat3(ViewMatrix * ModelMatrix); // Take the upper-left part of ModelViewMatrix
		Renderer::SetTextureShader(MVP, ModelMatrix, ViewMatrix, ModelView3x3Matrix);
		decal->RenderDecal(programid);
	}

	glDisable(GL_BLEND);
	std::ostringstream oss;
	oss.precision(2);
	glm::vec3 pos = btToGlmVector3(AssetManager::GetGameObject("player")->GetRigidBody()->getWorldTransform().getOrigin());
	oss << "Pos x:" << pos.x << " y:" << pos.y << " z:" << pos.z;
	Renderer::RenderText(oss.str().c_str(), 0, 570, 15);
	glm::vec3 vel = btToGlmVector3(AssetManager::GetGameObject("player")->GetRigidBody()->getTotalForce());
	oss.str(""); oss.clear();
	oss.precision(2);
	oss << "Vel x:" << vel.x << " y:" << vel.y << " z:" << vel.z;
	Renderer::RenderText(oss.str().c_str(), 0, 540, 15);
	if (Player::getCurrentGun() != "nothing")
	{
		oss.str(""); oss.clear();
		oss << WeaponManager::GetGunByName(Player::getCurrentGun())->currentammo << "/" << WeaponManager::GetGunByName(Player::getCurrentGun())->ammo;
		Renderer::RenderText(oss.str().c_str(), 660, 0, 15);
	}
	oss.str(""); oss.clear();
	oss << Player::getCurrentGun();
	Renderer::RenderText(oss.str().c_str(), 0, 500, 15);
	
}
void Scene::AddGunPickUp(GunPickUp gunpickup) {
	gunPickUps.push_back(gunpickup);
}
void Scene::AddGunPickUp(std::string gunName, std::string gunObject, glm::vec3 Position) {
	gunPickUps.push_back(GunPickUp(gunName, gunObject, Position));
}

int Scene::GetGunPickUpSize() {
	return gunPickUps.size();
}
Crate* Scene::GetCrate(std::string name) {
	for (int i = 0; i < crates.size(); i++) {
		if (crates[i].GetName() == name)
			return &crates[i];
	}
	return NULL;
}



