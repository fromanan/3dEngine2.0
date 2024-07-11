#include "Gun.h"
#include "Engine/Core/AssetManager.h"


void Gun::ReloadingAnimation(float deltaTime) {
	if (rotation > 0.8)
		down = -1;
	float incerment = (1.6 / reloadtime) * down * deltaTime;
	rotation += incerment;
	float currentXRotation = AssetManager::GetGameObject(gunModel)->getRotation().x;
	AssetManager::GetGameObject(gunModel)->SetRotationX(currentXRotation + rotation);
}
void Gun::Update(float deltaTime, glm::vec3 position) {
	kickbackOffset = kickbackOffset * 0.96;
	if (kickbackOffset < 0.001) kickbackOffset = 0;

	float verticalAngle = -AssetManager::GetGameObject(gunModel)->getRotation().x;
	float horizontalAngle = AssetManager::GetGameObject(gunModel)->getRotation().y;

	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	AssetManager::GetGameObject(gunModel)->setPosition(position + (direction * -kickbackOffset * deltaTime));

}
void Gun::Shoot() {
	kickbackOffset += kickback;
}

namespace WeaponManager {
	std::vector<Gun> guns;

	void WeaponManager::Init() {
		AssetManager::AddGameObject("pistol", "Assets/Objects/glock.obj", AssetManager::GetTexture("uvmap"), glm::vec3(0, 0, 0), false);

		AssetManager::AddTexture("ak47", "Assets/Textures/ak47.png");
		AssetManager::AddGameObject("ak47", "Assets/Objects/ak47.obj", AssetManager::GetTexture("ak47"), glm::vec3(0, 0, 0), false);

		Gun pistol;
		pistol.name = "pistol";
		pistol.ammo = 18;
		pistol.reloadtime = 1.5;
		pistol.firerate = 600;
		pistol.currentammo = 18;
		pistol.damage = 10;
		pistol.type = Auto;
		pistol.recoil = 0.01;
		pistol.recoilY = 100;
		pistol.kickback = 6;
		pistol.gunModel = "pistol";  
		guns.emplace_back(pistol);

		Gun ak47;
		ak47.name = "ak47";
		ak47.ammo = 30;
		ak47.reloadtime = 2.5;
		ak47.firerate = 600;
		ak47.currentammo = 30;
		ak47.damage = 25;
		ak47.type = Auto;
		ak47.recoil = 0.03;
		ak47.recoilY = 175;
		ak47.kickback = 6;
		ak47.gunModel = "ak47"; 
		guns.emplace_back(ak47);

	}
	Gun* WeaponManager::GetGunByName(std::string name) {
		for (int i = 0; i < guns.size(); i++)
			if (guns[i].name == name)
				return &guns[i];
		return NULL;
	}
}