#include "pch.h"

#include "Scene.h"

#include "AssetManager.h"
#include "AssetPaths.h"
#include "Camera.h"
#include "PhysicsManager.h"
#include "Renderer.h"
#include "Audio/AudioManager.h"
#include "Game/Player.h"
#include "Skybox.h"
#include "Game/Gun.h"
#include "Game/WeaponManager.h"

Scene::Scene()  = default;

void Scene::Load() {
	AssetManager::AddTexture("uvmap", AssetPaths::UV_Crate);
	AssetManager::AddTexture("crate", AssetPaths::Texture_Crate);

	AssetManager::AddTexture("target", AssetPaths::Texture_Target);
	AssetManager::AddTexture("container", AssetPaths::Texture_Container, AssetPaths::Normal_Container);
	AssetManager::AddTexture("bullet_hole", AssetPaths::Texture_BulletHole);
	AssetManager::AddTexture("bullet_hole_glass", AssetPaths::Texture_BulletHoleGlass);
	AssetManager::AddTexture("sand", AssetPaths::Texture_GroundSand, AssetPaths::Normal_GroundSand);

	AssetManager::AddTexture("concrete", AssetPaths::Texture_Fence, AssetPaths::Normal_Fence);
	AssetManager::AddTexture("ak47_lowpoly", AssetPaths::Texture_Ak47_LowPoly, AssetPaths::Normal_Ak47_LowPoly);
	AssetManager::AddTexture("crosshair", AssetPaths::Texture_Crosshair);
	AssetManager::AddTexture("window", AssetPaths::Texture_Window);
	
	AssetManager::AddTexture("glock", AssetPaths::Texture_Glock17, AssetPaths::Normal_Glock17);
	AssetManager::AddTexture("door2", AssetPaths::Texture_Door);

	// TODO: not currently working
	//AssetManager::LoadAssets(AssetPaths::Json_MainScene);

	WeaponManager::Init();
	AssetManager::AddGameObject("fence1", AssetPaths::Model_Fence_3, AssetManager::GetTexture("concrete"), glm::vec3(5, 1.3, 2), true, 0, Box);
	AssetManager::AddGameObject("fence2", AssetPaths::Model_Fence_1, AssetManager::GetTexture("concrete"), glm::vec3(-9, 1.3, 2), true, 0, Box);
	AssetManager::AddGameObject("fence3", AssetPaths::Model_Fence_2, AssetManager::GetTexture("concrete"), glm::vec3(2, 1.3, -9), true, 0, Box);
	AssetManager::AddGameObject("fence4", AssetPaths::Model_Fence_2, AssetManager::GetTexture("concrete"), glm::vec3(-1, 1.3, 5), true, 0, Box);
	//AssetManager::AddGameObject("floor", AssetPaths::Model_TestPlatform, AssetManager::GetTexture("sand"), glm::vec3(0, -2, 0), true, 0, Box);
	AssetManager::AddGameObject("floor", AssetPaths::Model_Floor, AssetManager::GetTexture("sand"), glm::vec3(0, 0, 0), true, 0, Box);

	AssetManager::AddGameObject("floor", AssetPaths::Model_Slope, AssetManager::GetTexture("sand"), glm::vec3(-1, 2, -7), true, 0, Convex);

	crates.emplace_back(glm::vec3(1, 25, 1), "crate2", AssetPaths::Model_Crate, AssetManager::GetTexture("crate"));
	crates.emplace_back(glm::vec3(1, 30, 0.5), "crate2", AssetPaths::Model_Crate, AssetManager::GetTexture("crate"));
	crates.emplace_back(glm::vec3(0.5, 20, 1), "crate2", AssetPaths::Model_Crate, AssetManager::GetTexture("crate"));

	gunPickUps.emplace_back("ak47", "ak47_pickup", AssetPaths::Model_Ak47, AssetManager::GetTexture("ak47"), glm::vec3(1, 30, 1));

	gunPickUps.emplace_back("glock", "glock_pickup1", AssetPaths::Model_Glock17, AssetManager::GetTexture("glock"), glm::vec3(1,25, 0));

	doors.emplace_back("door1", AssetPaths::Model_Door, AssetPaths::Model_Frame, AssetManager::GetTexture("door2"), AssetManager::GetTexture("door2"), glm::vec3(-3, 0, -3));

	/*gunPickUps.emplace_back("ak47", "ak47_pickup", AssetPaths::Model_Ak47_LowPoly, AssetManager::GetTexture("ak47_lowpoly"), glm::vec3(8, -12, -5));
	AssetManager::GetGameObject("ak47_pickup")->SetRender(false);
	gunPickUps.emplace_back("glock", "glock_pickup", AssetPaths::Model_Glock17, AssetManager::GetTexture("uvmap"), glm::vec3(8, -13, -6));
	AssetManager::GetGameObject("glock_pickup")->SetRender(false);*/

	// Sets renderer
	Renderer::UseProgram(Renderer::GetProgramID("Texture"));
	std::vector<std::string> faces { 
		AssetPaths::Texture_SkyboxRight,
			AssetPaths::Texture_SkyboxLeft,
			AssetPaths::Texture_SkyboxTop,
			AssetPaths::Texture_SkyboxBottom,
			AssetPaths::Texture_SkyboxFront,
		AssetPaths::Texture_SkyboxBack
	};
	sky = Skybox(faces);

	// MAX LIGHTS BY DEFAULT IS 10 if you want more lights go to FragmentShader.frag and VertexShader.vert and change MAXLIGHTS
	lights.emplace_back(glm::vec3(-2.5, 4, -5), glm::vec3(1, 0.25, 0), 1,0.22f, 0.20f);
	lights.emplace_back(glm::vec3(-6, 2, -2), glm::vec3(1, 0, 1), 1, 0.22f, 0.20f);
	lights.emplace_back(glm::vec3(-1, 2, -1), glm::vec3(0, 1, 1), 1, 0.22f, 0.20f);

	Player::Init();
	Player::setPosition(glm::vec3(3, 10, 0));

	// TODO: not currently working
	//AssetManager::SaveAssets(AssetPaths::Json_MainScene);
}

void Scene::Update(const float deltaTime) {
	Player::Update(deltaTime);

	for (int i = 0; i < AssetManager::GetGameObjectsSize(); i++) {
		AssetManager::GetGameObject(i)->Update();
	}
	
	for (auto& door : doors) {
		door.Interact();
		door.Update(deltaTime);
	}
	
	for (int gun = 0; gun < gunPickUps.size(); gun++) {
		gunPickUps[gun].Update();
		if (gunPickUps[gun].Interact() && Player::getCurrentGun() == Tags::NOTHING)
			gunPickUps.erase(gunPickUps.begin() + gun);
	}
	
	for (const auto& crate : crates) {
		crate.Update();
	}
	
	AudioManager::UpdateListener(Player::getPosition(), Player::getForward(), PhysicsManager::GetRigidBody("PlayerRB")->GetForce());
	AudioManager::Update();
}

void Scene::RenderObjects() const {
	const glm::mat4 ProjectionMatrix = Camera::getProjectionMatrix();
	const glm::mat4 ViewMatrix = Camera::getViewMatrix();
	const glm::mat4 PV = ProjectionMatrix * ViewMatrix;

	Renderer::RendererSkybox(ViewMatrix, ProjectionMatrix, sky);
	Renderer::UseProgram(Renderer::GetProgramID("Texture"));
	
	GLuint programid = Renderer::GetCurrentProgramID();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	Renderer::SetLights(lights);

	for (int i = 0; i < AssetManager::GetGameObjectsSize(); i++) {
		GameObject* gameObjectRender = AssetManager::GetGameObject(i);

		if (!gameObjectRender->ShouldRender())
			continue;

		glm::mat4 ModelMatrix = gameObjectRender->GetModelMatrix();
		glm::mat4 MVP = PV * ModelMatrix;
		glm::mat3 ModelView3x3Matrix = glm::mat3(ViewMatrix * ModelMatrix); // Take the upper-left part of ModelViewMatrix

		Renderer::SetTextureShader(MVP, ModelMatrix, ViewMatrix, ModelView3x3Matrix);
		gameObjectRender->RenderObject(programid);
	}

	for (const auto& window : windows) {
		window.Render(Renderer::GetProgramID("Texture"), ViewMatrix, ProjectionMatrix);
	}

	for (int i = 0; i < AssetManager::GetDecalsSize(); i++) {
		const Decal* decal = AssetManager::GetDecal(i);
		if (decal->CheckParentIsNull())
			continue;
		// Do some pre-normal calculations
		glm::mat4 ModelMatrix = decal->GetModel();
		glm::mat4 MVP = PV * ModelMatrix;
		glm::mat3 ModelView3x3Matrix = glm::mat3(ViewMatrix * ModelMatrix); // Take the upper-left part of ModelViewMatrix
		Renderer::SetTextureShader(MVP, ModelMatrix, ViewMatrix, ModelView3x3Matrix);
		decal->RenderDecal(programid);
	}

	glDisable(GL_BLEND);
	std::ostringstream oss;
	oss.precision(2);
	const glm::vec3 pos = btToGlmVector3(AssetManager::GetGameObject("player")->GetRigidBody()->getWorldTransform().getOrigin());
	
	oss << "Pos x:" << pos.x << " y:" << pos.y << " z:" << pos.z;
	Renderer::RenderText(oss.str().c_str(), 0, 570, 15);

	const glm::vec3 vel = btToGlmVector3(AssetManager::GetGameObject("player")->GetRigidBody()->getLinearVelocity());
	oss.str("");
	oss.clear();
	oss.precision(2);
	oss << "Vel x:" << vel.x << " y:" << vel.y << " z:" << vel.z;
	Renderer::RenderText(oss.str().c_str(), 0, 540, 15);
	
	if (Player::getCurrentGun() != Tags::NOTHING) {
		oss.str("");
		oss.clear();
		const int currentAmmo = WeaponManager::GetGunByName(Player::getCurrentGun())->currentammo;
		const int ammo = WeaponManager::GetGunByName(Player::getCurrentGun())->ammo;
		oss << currentAmmo << "/" << ammo;
		Renderer::RenderText(oss.str().c_str(), 660, 0, 15);
	}
	oss.str(""); oss.clear();
	oss << Player::getCurrentGun();
	Renderer::RenderText(oss.str().c_str(), 0, 500, 15);
}

void Scene::AddGunPickUp(const GunPickup& gunpickup) {
	gunPickUps.push_back(gunpickup);
}

void Scene::AddGunPickUp(const std::string& gunName, const std::string& gunObject, const glm::vec3 Position) {
	const GunPickup pickup = GunPickup(gunName, gunObject, Position);
	std::cout << " testing";
	gunPickUps.push_back(pickup);
}

void Scene::RenderObjects(const char* shaderName) const {
	const glm::mat4 ProjectionMatrix = Camera::getProjectionMatrix();
	const glm::mat4 ViewMatrix = Camera::getViewMatrix();
	const glm::mat4 PV = ProjectionMatrix * ViewMatrix;

	Renderer::RendererSkybox(ViewMatrix, ProjectionMatrix, sky);
	GLuint programid = Renderer::GetProgramID(shaderName);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Renderer::SetLights(lights);

	for (int i = 0; i < AssetManager::GetGameObjectsSize(); i++) {
		const GameObject* gameObjectRender = AssetManager::GetGameObject(i);

		if (!gameObjectRender->ShouldRender())
			continue;

		glm::mat4 ModelMatrix = gameObjectRender->GetModelMatrix();
		glm::mat4 MVP = PV * ModelMatrix;
		glm::mat3 ModelView3x3Matrix = glm::mat3(ViewMatrix * ModelMatrix); // Take the upper-left part of ModelViewMatrix

		Renderer::SetTextureShader(MVP, ModelMatrix, ViewMatrix, ModelView3x3Matrix);
		gameObjectRender->RenderObject(programid);
	}
}

size_t Scene::GetGunPickUpSize() const {
	return gunPickUps.size();
}

Crate* Scene::GetCrate(const std::string& name) {
	for (auto& crate : crates) {
		if (crate.GetName() == name)
			return &crate;
	}
	return nullptr;
}
