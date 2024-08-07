#pragma once
#include "Skybox.h"
#include "Game/Crate.h"
#include "Game/Door.h"
#include "Game/GunPickup.h"
#include "Game/Window.h"
#include "Lights/Light.h"

class Scene
{
public:
	Scene();

	void Load();
	void Update(float dt);
	void RenderObjects() const;
	void RenderObjects(const char* shaderName) const;

	void AddGunPickUp(const GunPickup& gunpickup);
	void AddGunPickUp(const std::string& gunName, const std::string& gunObject,glm::vec3 Position);

	size_t GetGunPickUpSize() const;
	Crate* GetCrate(const std::string& name);
	Model* GetModel(const std::string& name);
private:
	Skybox sky = Skybox();
	// Objects
	std::vector<Door> doors;
	std::vector<Crate> crates;
	std::vector<GunPickUp> gunPickUps;
	std::vector<Light> lights;

	//holds the models
	std::map<std::string, Model> models;
};
