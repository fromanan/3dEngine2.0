#pragma once
#include <string>
#include <vector>
#include "Engine/Core/Camera.h"
#include "Engine/Core/GameObject.h"
#include "Engine/Game/Player.h"



enum GunType {
	Semi,
	Auto,
	ShotGun
};

struct Gun {
	std::string name;
	int ammo;
	double firerate;
	int currentammo;
	double reloadtime;
	int damage;
	float recoil;
	float recoilY;
	float kickback;
	double lastTimeShot = glfwGetTime();
	std::string gunModel;
	GunType type;

	float kickbackOffset = 0;
	double rotation = 0;
	int down = 1;
	void ReloadingAnimation(float deltaTime);
	void Update(float deltaTime, glm::vec3 position);
	void Shoot();
};

namespace WeaponManager {
	void Init();
	Gun* GetGunByName(std::string name);
}

class GunPickUp {
public:
	GunPickUp(std::string GunName, std::string ObjectName, const char* objectModel, Texture* texture, glm::vec3 position);
	bool Interact();
private:
	std::string gunName;
	std::string objectName;

};
