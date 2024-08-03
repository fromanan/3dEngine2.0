#include "pch.h"

#include "GunPickup.h"

#include "AssetManager.h"
#include "Gun.h"
#include "PhysicsManagerBullet.h"
#include "Player.h"
#include "WeaponManager.h"
#include "Audio/AudioManager.h"
#include "Scene/SceneManager.h"

GunPickup::GunPickup(const std::string& gunName, const std::string& objectName, const char* objectModel, Texture* texture, const glm::vec3 position) {
    this->gunName = gunName;
    this->objectName = objectName;
    AssetManager::AddGameObject(this->objectName, objectModel, texture, position, false,1,Convex);
}

// TODO: Doesn't work
GunPickup::GunPickup(const std::string& gunName, const std::string& gunObject, const glm::vec3 position) {
    this->gunName = gunName;
    this->objectName = gunObject + std::to_string(SceneManager::GetCurrentScene()->GetGunPickUpSize());
    AssetManager::GetGameObject(gunObject)->Copy(objectName);
    AssetManager::GetGameObject(objectName)->SetRender(true);
    AssetManager::GetGameObject(objectName)->setPosition(position);
}

void GunPickup::Update() {
	
}

bool GunPickup::Interact() const {
    if (Player::GetInteractingWithName() != objectName || !Player::SelectWeapon(gunName))
        return false;
	
    GameObject* object = AssetManager::GetGameObject(objectName);

    PhysicsManagerBullet::GetDynamicWorld()->removeRigidBody(object->GetRigidBody());
    object->SetRender(false);
		
    WeaponManager::GetGunByName(gunName)->currentammo = WeaponManager::GetGunByName(gunName)->ammo;
    AudioManager::PlaySound("item_pickup", Player::getPosition());
    return true;
}
