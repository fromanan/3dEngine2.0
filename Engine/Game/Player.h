#pragma once

namespace Player
{
	extern GameObject* playerModel;
	extern GameObject* gun;
	glm::vec3 getPosition();
	glm::vec3 getForward();
	void setPosition(glm::vec3 pos);
	void Update(float deltaTime);
	void Init();
	std::string GetInteractingWithName();
	std::string getCurrentGun();

	bool SelectWeapon(const std::string& weaponName);
	bool OnGround();

	void SwitchWeapons(int index);
	void Shoot();
}
