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
	int reloadtime;
	int damage;
	float recoil;
	float kickback;
	double lastTimeShot = glfwGetTime();
	GameObject* gunModel;
	GunType type;
};

namespace WeaponManager {
	void Init();
	Gun* GetGunByName(std::string name);
}
