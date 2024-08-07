#include "pch.h"

#include "WeaponManager.h"

#include "AssetManager.h"
#include "AssetPaths.h"
#include "Audio/AudioManager.h"

namespace WeaponManager
{
	std::vector<Gun> guns;

	void WeaponManager::Init() {
		AssetManager::AddGameObject(GameObject("glock", SceneManager::GetCurrentScene()->GetModel("glock"), glm::vec3(0.2, -0.25, 0.2), false, 0, Box, 0, 0, 0));
		//AssetManager::AddGameObject(GameObject("glock", AssetPaths::Model_Glock17, AssetManager::GetTexture("glock"), glm::vec3(0.2, -0.25, 0.2), false, 0, Box, 0, 0, 0));
		AssetManager::GetGameObject("glock")->SetRender(false);
		AssetManager::GetGameObject("glock")->SetParentName("player_head");

		AssetManager::AddGameObject(GameObject("ak47", SceneManager::GetCurrentScene()->GetModel("ak47"), glm::vec3(0.2, -0.25, -0.2), false, 0, Box, 0, 0, 0));
		//AssetManager::AddTexture("ak47", AssetPaths::Texture_Ak47, AssetPaths::Normal_Ak47);
		//AssetManager::AddGameObject(GameObject("ak47", AssetPaths::Model_Ak47, AssetManager::GetTexture("ak47"), glm::vec3(0.2, -0.25, -0.2), false, 0, Box, 0, 0, 0));
		AssetManager::GetGameObject("ak47")->SetRender(false);
		AssetManager::GetGameObject("ak47")->SetParentName("player_head");
		
		AudioManager::AddSound(AssetPaths::Audio_Ak47_1, "ak47_fire1", AssetManager::GetGameObject("ak47")->getPosition(), 5, 0.5f);
		AudioManager::AddSound(AssetPaths::Audio_Ak47_2, "ak47_fire2", AssetManager::GetGameObject("ak47")->getPosition(), 5, 0.5f);
		AudioManager::AddSound(AssetPaths::Audio_Ak47_3, "ak47_fire3", AssetManager::GetGameObject("ak47")->getPosition(), 5, 0.5f);
		AudioManager::AddSound(AssetPaths::Audio_Ak47_4, "ak47_fire4", AssetManager::GetGameObject("ak47")->getPosition(), 5, 0.5f);
		AudioManager::AddSound(AssetPaths::Audio_Glock17_1, "glock_fire1", AssetManager::GetGameObject("glock")->getPosition(), 5, 0.5f);
		AudioManager::AddSound(AssetPaths::Audio_Glock17_2, "glock_fire2", AssetManager::GetGameObject("glock")->getPosition(), 5, 0.5f);
		AudioManager::AddSound(AssetPaths::Audio_Glock17_3, "glock_fire3", AssetManager::GetGameObject("glock")->getPosition(), 5, 0.5f);
		AudioManager::AddSound(AssetPaths::Audio_Glock17_4, "glock_fire4", AssetManager::GetGameObject("glock")->getPosition(), 5, 0.5f);
		AudioManager::AddSound(AssetPaths::Audio_DryFire, "dry_fire", AssetManager::GetGameObject("glock")->getPosition(), 5, 0.2f);
		
		Gun glock;
		glock.name = "glock";
		glock.ammo = 18;
		glock.reloadtime = 1.5;
		glock.firerate = 250;
		glock.currentammo = 18;
		glock.damage = 10;
		glock.type = Semi;
		glock.recoil = 0.01f;
		glock.recoilY = 100;
		glock.kickback = 3;
		glock.weaponOffSet = glm::vec3(-0.3, -0.2, 0.9);
		glock.aimingPosition = glm::vec3(0,-0.16, 0.7);
		glock.gunModel = "glock"; 
		glock.gunsShotName = "glock_fire";
		guns.emplace_back(glock);

		Gun ak47;
		ak47.name = "ak47";
		ak47.ammo = 30;
		ak47.reloadtime = 2.5;
		ak47.firerate = 600;
		ak47.currentammo = 30;
		ak47.damage = 25;
		ak47.type = Auto;
		ak47.recoil = 0.03f;
		ak47.recoilY = 175;
		ak47.kickback = 2;
		ak47.gunModel = "ak47";
		ak47.gunsShotName = "ak47_fire";
		ak47.weaponOffSet = glm::vec3(-0.3, -0.25, 0.9);
		ak47.aimingPosition = glm::vec3(0, -0.2, 0.7);
		guns.emplace_back(ak47);
	}
	
	Gun* WeaponManager::GetGunByName(const std::string& name) {
		for (auto& gun : guns)
			if (gun.name == name)
				return &gun;
		return nullptr;
	}
}
