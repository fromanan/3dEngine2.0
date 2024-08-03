#include "pch.h"

#include "Gun.h"

#include "AssetManager.h"
#include "AssetPaths.h"
#include "Player.h"
#include "Audio/AudioManager.h"
#include "Scene/SceneManager.h"

void Gun::Update(const float deltaTime, const bool isReloading, const bool aiming) {
	GameObject* gun = AssetManager::GetGameObject(gunModel);
	gun->GetRigidBody()->setAngularVelocity(btVector3(0, 0, 0));
	gun->GetRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
	kickbackOffset *= 0.96f;
	if (kickbackOffset < 0.01)
		kickbackOffset = 0;

	float verticalAngle = -gun->getRotation().x;
	float horizontalAngle = gun->getRotation().y;

	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	if (isReloading) {
		const float currentXRotation = AssetManager::GetGameObject(gunModel)->getRotation().x;
		if (currentXRotation > 1.6 / 2)
			down = -1;
		const float increment = (1.6 / reloadtime) * down * deltaTime;
		gun->SetRotationX(currentXRotation + increment);
		gun->addPosition(glm::vec3(0, -increment / 3, 0));
	}
	else if (aiming) {
		AssetManager::GetGameObject(gunModel)->setPosition(aimingPosition);
	}
	else {
		gun->setPosition(weaponOffSet + (direction * -kickbackOffset * deltaTime));
		gun->SetRotationX(0);
	}
	
	AudioManager::GetSound(gunsShotName + std::to_string(1))->SetPosition(Player::getPosition());
	AudioManager::GetSound(gunsShotName + std::to_string(2))->SetPosition(Player::getPosition());
	AudioManager::GetSound(gunsShotName + std::to_string(3))->SetPosition(Player::getPosition());
	AudioManager::GetSound(gunsShotName + std::to_string(4))->SetPosition(Player::getPosition());
}

void Gun::Shoot() {
	int randomnum = (rand() % 4) + 1;
	AudioManager::PlaySound(gunsShotName + std::to_string(randomnum));
	kickbackOffset += kickback;
}
