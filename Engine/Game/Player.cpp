#include "Player.h"
#include "Engine/Core/AssetManager.h"
#include "Engine/Core/Scene/SceneManager.h"
#include "Engine/Physics/BulletPhysics.h"



namespace Player
{
	glm::vec3 forward;
	float horizontalAngle = 3.14f;
	float verticalAngle = 0.0f;
	float initialFoV = 45.0f;
	float maxAngle = 1.5;
	float mouseSpeed = 0.005f;
	float speed = 4;
	float jumpforce = 50;
	std::string gunName = "";
	std::string interactingWithName = "nothing";
	float interactDistance = 2;

	//states
	bool reloading = false;
	bool aiming = false;

	double reloadingTime = 0;
	double footstepTime = 0;
	double footstep_interval = 0.5;

	void Player::Init() {
		srand(time(0));
		AssetManager::AddGameObject(GameObject("player", "Assets/Objects/capsule.obj", AssetManager::GetTexture("uvmap"), glm::vec3(0, 10, 5), false,1,Box,0.5,2,0.5));
		AssetManager::GetGameObject("player")->SetRender(false);
		std::cout << "loading player model" << std::endl;
		gunName = "nothing";
	}
	void Player::Shoot() {
		if (WeaponManager::GetGunByName(gunName)->currentammo > 0)
		{
			WeaponManager::GetGunByName(gunName)->currentammo--;
			WeaponManager::GetGunByName(gunName)->Shoot();
			verticalAngle += WeaponManager::GetGunByName(gunName)->recoil;
			horizontalAngle += (((double)rand()) / RAND_MAX) / WeaponManager::GetGunByName(gunName)->recoilY;
			if (Camera::GetLookingAtCollider()->GetStatic())
			{
				if (Camera::GetLookingAtCollider()->GetTag() == "glass") {
					AssetManager::AddDecal(Camera::GetRay().origin + Camera::GetLookingAtDistance() * Camera::GetRay().direction, Camera::GetRayInfo()->normal, glm::vec3(0.025, 0.025, 0.025), AssetManager::GetTexture("bullet_hole_glass"));
					AssetManager::AddDecal(Camera::GetRay().origin + Camera::GetLookingAtDistance() * Camera::GetRay().direction, -1.0f * Camera::GetRayInfo()->normal, glm::vec3(0.02, 0.02, 0.02), AssetManager::GetTexture("bullet_hole_glass"));
					if(Camera::GetRayInfo2()->collider->GetStatic())
						AssetManager::AddDecal(Camera::GetRay().origin + (Camera::GetRayInfo2()->distance - 0.015f) * Camera::GetRay().direction, Camera::GetRayInfo2()->normal, glm::vec3(0.02, 0.02, 0.02), AssetManager::GetTexture("bullet_hole"));
					AudioManager::PlaySound("glass_impact" + std::to_string((rand() % 2) + 1), Camera::GetRay().origin + Camera::GetLookingAtDistance() * Camera::GetRay().direction);
				}
				else if (Camera::GetRayInfo2()->collider->GetStatic())
				{
					AssetManager::AddDecal(Camera::GetRay().origin + Camera::GetLookingAtDistance() * Camera::GetRay().direction, Camera::GetRayInfo()->normal, glm::vec3(0.02, 0.02, 0.02), AssetManager::GetTexture("bullet_hole"));
				}
			}

			if (SceneManager::GetCurrentScene()->GetCrate(Camera::GetLookingAtName()) != NULL) {
				glm::vec3 force = Camera::GetRay().origin + (Camera::GetLookingAtDistance() * 1) * Camera::GetRay().direction - Camera::GetRay().origin + Camera::GetLookingAtDistance() * Camera::GetRay().direction;
				SceneManager::GetCurrentScene()->GetCrate(Camera::GetLookingAtName())->DealDamage(WeaponManager::GetGunByName(gunName)->damage, Camera::GetRay().origin + Camera::GetLookingAtDistance() * Camera::GetRay().direction,force * 10.0f);
			}
			else if (SceneManager::GetCurrentScene()->GetCrate(Camera::GetRayInfo2()->name) != NULL && Camera::GetLookingAtCollider()->GetTag() == "glass")
			{
				glm::vec3 force = Camera::GetRay().origin + (Camera::GetLookingAtDistance() * 1) * Camera::GetRay().direction - Camera::GetRay().origin + Camera::GetLookingAtDistance() * Camera::GetRay().direction;
				SceneManager::GetCurrentScene()->GetCrate(Camera::GetLookingAtName())->DealDamage(WeaponManager::GetGunByName(gunName)->damage, Camera::GetRay().origin + Camera::GetLookingAtDistance() * Camera::GetRay().direction, force * 10.0f);
			}
		}
		else {
			//click click
			AudioManager::PlaySound("dry_fire", AssetManager::GetGameObject("player")->getPosition());
		}
		WeaponManager::GetGunByName(gunName)->lastTimeShot = glfwGetTime();
	}

	void Player::Update(float deltaTime) {
		interactingWithName = "Nothing";

		if (verticalAngle <= maxAngle && verticalAngle >= -maxAngle)
			verticalAngle += mouseSpeed * float(768 / 2 - Input::GetMouseY());
		
		else if (verticalAngle > maxAngle)
			verticalAngle = maxAngle;
		
		else if (verticalAngle < -maxAngle)
			verticalAngle = -maxAngle;
		
		forward = glm::vec3(
			sin(horizontalAngle + 3.14f),
			0,
			cos(horizontalAngle + 3.14f)
		);
		// Right vector
		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);
		// Move forward
		if (Input::KeyDown('w')) {
			AssetManager::GetGameObject("player")->GetRigidBody()->getWorldTransform().setOrigin(glmToBtVector3(-forward * speed * deltaTime) + AssetManager::GetGameObject("player")->GetRigidBody()->getWorldTransform().getOrigin());
			//AssetManager::GetGameObject("player")->GetRigidBody()->applyCentralForce(glmToBtVector3(-forward * speed * deltaTime));
		}
		// Move backward
		if (Input::KeyDown('s')) {
			AssetManager::GetGameObject("player")->GetRigidBody()->getWorldTransform().setOrigin(glmToBtVector3(forward * speed * deltaTime) + AssetManager::GetGameObject("player")->GetRigidBody()->getWorldTransform().getOrigin());
			//AssetManager::GetGameObject("player")->GetRigidBody()->applyCentralForce(glmToBtVector3(forward * speed * deltaTime));
		}
		// Strafe right
		if (Input::KeyDown('d')) {
			AssetManager::GetGameObject("player")->GetRigidBody()->getWorldTransform().setOrigin(glmToBtVector3(right * speed * deltaTime) + AssetManager::GetGameObject("player")->GetRigidBody()->getWorldTransform().getOrigin());
			//AssetManager::GetGameObject("player")->GetRigidBody()->applyCentralForce(glmToBtVector3(right * speed * deltaTime));
		}
		// Strafe left
		if (Input::KeyDown('a')) {
			AssetManager::GetGameObject("player")->GetRigidBody()->getWorldTransform().setOrigin(glmToBtVector3(-right * speed * deltaTime) + AssetManager::GetGameObject("player")->GetRigidBody()->getWorldTransform().getOrigin());
			//AssetManager::GetGameObject("player")->GetRigidBody()->applyCentralForce(glmToBtVector3(-right * speed * deltaTime));
		}
		if (Input::KeyPressed('e') && Camera::GetLookingAtDistance() <= interactDistance) {
			interactingWithName = Camera::GetLookingAtName();
		}
		if (Input::KeyPressed('r') && !reloading && !aiming) {
			reloading = true;
			reloadingTime = glfwGetTime();
		}

		if (Input::RightMouseDown() && !reloading) {
			aiming = true;
		}
		else 
			aiming = false;
		

		if (gunName != "nothing"){
			
			if (glfwGetTime() - reloadingTime > WeaponManager::GetGunByName(gunName)->reloadtime && reloading)
			{
				reloading = false;
				WeaponManager::GetGunByName(gunName)->currentammo = WeaponManager::GetGunByName(gunName)->ammo;
				WeaponManager::GetGunByName(gunName)->down = 1;
			}
			//get ray details
			if (Input::LeftMousePressed() && Camera::GetLookingAtDistance() < 9999 && WeaponManager::GetGunByName(gunName)->type == Semi && glfwGetTime() - WeaponManager::GetGunByName(gunName)->lastTimeShot > 60.0f / WeaponManager::GetGunByName(gunName)->firerate && !reloading) {
				Shoot();
			}
			if (Input::LeftMouseDown() && Camera::GetLookingAtDistance() < 9999 && WeaponManager::GetGunByName(gunName)->type == Auto && glfwGetTime() - WeaponManager::GetGunByName(gunName)->lastTimeShot > 60.0f / WeaponManager::GetGunByName(gunName)->firerate && !reloading) {
				Shoot();
			}
			if (Input::KeyPressed('q') && !reloading) {
				//SceneManager::GetCurrentScene()->AddGunPickUp(gunName, gunName + "_pickup", rb->GetPostion() + Camera::GetDirection() * 1.5f);
				AssetManager::GetGameObject(gunName)->SetRender(false);
				gunName = "nothing";
			}
		}

		horizontalAngle += mouseSpeed * float(1024 / 2 - Input::GetMouseX());

		Camera::SetHorizontalAngle(horizontalAngle);
		Camera::SetVerticalAngle(verticalAngle);
		Camera::SetPosition(AssetManager::GetGameObject("player")->getPosition());

		//PhysicsManager::GetColider("PlayerCollider")->setPosition(rb->GetPostion());

		AssetManager::GetGameObject("player")->SetRotationX(-verticalAngle);
		AssetManager::GetGameObject("player")->SetRotationY(horizontalAngle);
		//AssetManager::GetGameObject("player")->GetRigidBody()->getWorldTransform().setRotation(glmToBtVector3())
		//AssetManager::GetGameObject("player")->setPosition();
		if(gunName != "nothing")
			WeaponManager::GetGunByName(gunName)->Update(deltaTime, reloading, aiming);

		if ((Input::KeyDown('w') || Input::KeyDown('a') || Input::KeyDown('s') || Input::KeyDown('d')) && footstepTime + footstep_interval < glfwGetTime() ) {
			AudioManager::PlaySound("foot_step" + std::to_string((rand() % 4) + 1), AssetManager::GetGameObject("player")->getPosition());
			footstepTime = glfwGetTime();
		}
		
		
		
	}
	glm::vec3 Player::getPosition() {
		return AssetManager::GetGameObject("player")->getPosition();
	}
	glm::vec3 Player::getForward() {
		return forward;
	}
	void Player::setPosition(glm::vec3 pos) {
		AssetManager::GetGameObject("player")->setPosition(pos);
		Camera::SetPosition(AssetManager::GetGameObject("player")->getPosition());
	}
	std::string Player::GetInteractingWithName() {
		return interactingWithName;
	}
	std::string Player::getCurrentGun() {
		return gunName;
	}
	bool Player::SelectWeapon(std::string weaponName) {
		if (reloading)
			return false;
		if(gunName != "nothing")
			AssetManager::GetGameObject(WeaponManager::GetGunByName(gunName)->gunModel)->SetRender(false);
		gunName = weaponName;
		AssetManager::GetGameObject(WeaponManager::GetGunByName(gunName)->gunModel)->SetRender(true);
		return true;
	}

}