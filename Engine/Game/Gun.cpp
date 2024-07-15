#include "Gun.h"
#include "Engine/Core/AssetManager.h"



void Gun::Update(float deltaTime, bool isReloading, bool aiming) {
	kickbackOffset = kickbackOffset * 0.96;
	if (kickbackOffset < 0.01) kickbackOffset = 0;

	float verticalAngle = -AssetManager::GetGameObject(gunModel)->getRotation().x;
	float horizontalAngle = AssetManager::GetGameObject(gunModel)->getRotation().y;

	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	if (isReloading)
	{
		float currentXRotation = AssetManager::GetGameObject(gunModel)->getRotation().x;
		if (currentXRotation > 1.6 / 2)
			down = -1;			
		float incerment = (1.6 / reloadtime) * down * deltaTime;
		std::cout << currentXRotation << std::endl;
		AssetManager::GetGameObject(gunModel)->SetRotationX(currentXRotation + incerment);
		AssetManager::GetGameObject(gunModel)->addPosition(glm::vec3(0,-incerment/3,0));
	}
	else if (aiming) {
		AssetManager::GetGameObject(gunModel)->setPosition(aimingPosition);
	}
	else
	{
		AssetManager::GetGameObject(gunModel)->setPosition(weaponOffSet + (direction * -kickbackOffset * deltaTime));
		AssetManager::GetGameObject(gunModel)->SetRotationX(0);
	}
}
void Gun::Shoot() {
	kickbackOffset += kickback;
}

namespace WeaponManager {
	std::vector<Gun> guns;

	void WeaponManager::Init() {
		AssetManager::AddGameObject("pistol", "Assets/Objects/glock.obj", AssetManager::GetTexture("uvmap"), glm::vec3(0.2, -0.25, 0.2), false);
		AssetManager::GetGameObject("pistol")->SetParentName("player");

		AssetManager::AddTexture("ak47", "Assets/Textures/ak47.png","Assets/Normals/ak47_normal.png");
		AssetManager::AddGameObject("ak47", "Assets/Objects/ak47.obj", AssetManager::GetTexture("ak47"), glm::vec3(0.2, -0.25, -0.2), false);
		AssetManager::GetGameObject("ak47")->SetRender(false);
		AssetManager::GetGameObject("ak47")->SetParentName("player");



		Gun pistol;
		
		pistol.name = "pistol";
		pistol.ammo = 18;
		pistol.reloadtime = 1.5;
		pistol.firerate = 250;
		pistol.currentammo = 18;
		pistol.damage = 10;
		pistol.type = Semi;
		pistol.recoil = 0.01;
		pistol.recoilY = 100;
		pistol.kickback = 3;
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
		ak47.kickback = 2;
		ak47.gunModel = "ak47";
		ak47.weaponOffSet = glm::vec3(-0.3, -0.3, 0.7);
		ak47.aimingPosition = glm::vec3(0, -0.2, 0.7);
		guns.emplace_back(ak47);

	}
	Gun* WeaponManager::GetGunByName(std::string name) {
		for (int i = 0; i < guns.size(); i++)
			if (guns[i].name == name)
				return &guns[i];
		return NULL;
	}
}


GunPickUp::GunPickUp(std::string GunName, std::string ObjectName, const char* objectModel, Texture* texture, glm::vec3 position) {
	gunName = GunName;
	objectName = ObjectName;
	AssetManager::AddGameObject(objectName, objectModel, texture, position, false);
	AssetManager::GetGameObject(objectName)->SetRotationZ(1.5f);
	PhysicsManager::AddCube(glm::vec3(position.x,position.y,position.z + 0.5), 1, 0.3, 3, ObjectName);
	PhysicsManager::GetColider(objectName)->SetStatic(false);
	PhysicsManager::GetColider(objectName)->SetIsTrigger(true);

	
}
bool GunPickUp::Interact() {
	if (Player::GetInteractingWithName() == objectName && Player::getCurrentGun() != gunName && Player::SelectWeapon(gunName)) {
		AssetManager::RemoveGameObject(objectName);
		PhysicsManager::RemoveCube(objectName);
		WeaponManager::GetGunByName(gunName)->currentammo = WeaponManager::GetGunByName(gunName)->ammo;
		return true;
	}
	return false;
}