#pragma once
#include <iostream>
#include <vector>
#include <ostream>

#include "Engine/Audio/AudioManager.h"
#include "Engine/Core/AssetManager.h"
#include "Engine/Core/GameObject.h"
#include "Engine/Core/Renderer.h"
#include "Engine/Core/Camera.h"
#include "Engine/Core/Camera.h"
#include "Engine/Backend.h"
#include "Engine/Game/Player.h"
#include "Engine/Game/Door.h"
#include "Engine/Game/Gun.h"
#include "Engine/Core/Animation.h"
#include "Engine/Game/Door.h"
#include "Engine/Game/Gun.h"
#include "Engine/Game/Crate.h"
#include "Engine/Game/Window.h"
#include "Engine/Core/Lights/Light.h"
#include "Engine/Physics/BulletPhysics.h"
 
class Scene
{
public:
	Scene();

	void Load();
	void Update(float dt);
	void RenderObjects();
	void RenderObjects(const char* shaderName) const;

	void AddGunPickUp(const GunPickUp& gunpickup);
	void AddGunPickUp(const std::string& gunName, const std::string& gunObject,glm::vec3 Position);

	size_t GetGunPickUpSize() const;
	Crate* GetCrate(const std::string& name);
private:
	Skybox sky = Skybox();
	// Objects
	std::vector<Door> doors;
	std::vector<Crate> crates;
	std::vector<GunPickUp> gunPickUps;
	std::vector<Window> windows;
	std::vector<Light> lights;
};
