#pragma once
#include <string>
#include <vector>
#include "Engine/Core/GameObject.h"



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
	GameObject* gunModel;
	GunType type;

	
	double rotation = 0;
	int down = 1;
	void ReloadingAnimation(float deltaTime);
};

namespace WeaponManager {
	void Init();
	Gun* GetGunByName(std::string name);
}
