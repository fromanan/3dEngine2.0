#include "Player.h"
#include "AssetManager.h"

namespace Player
{
	float horizontalAngle = 3.14f;
	float verticalAngle = 0.0f;
	float initialFoV = 45.0f;
	float maxAngle = 1.5;
	float mouseSpeed = 0.005f;
	float speed = 40;
	float jumpforce = 50;
	RigidBody* rb;
	Cube* collider;
	std::string gunName = "";

	std::string interactingWithName = "Nothing";
	float interactDistance = 2;

	//states
	bool reloading = false;
	double reloadingTime = 0;

	void Player::Init() {
		srand(time(0));

		rb = PhysicsManager::AddRigidbody(glm::vec3(0, 0, 5), "PlayerRB");
		collider = PhysicsManager::AddCube(rb->GetPostion(), 0.5, 4, 0.5, "PlayerCollider");
		rb->SetColider(collider);
		std::cout << "loading player model" << std::endl;

		gunName = "pistol";
	}

	void Player::Update(float deltaTime) {
		interactingWithName = "Nothing";

		if (verticalAngle <= maxAngle && verticalAngle >= -maxAngle)
		{
			verticalAngle += mouseSpeed * float(768 / 2 - Input::GetMouseY());
		}
		else if (verticalAngle > maxAngle)
		{
			verticalAngle = maxAngle;
		}
		else if (verticalAngle < -maxAngle)
		{
			verticalAngle = -maxAngle;
		}
		glm::vec3 forward = glm::vec3(
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
			rb->AddForce(-forward * speed * deltaTime);
		}
		// Move backward
		if (Input::KeyDown('s')) {
			rb->AddForce(forward * speed * deltaTime);
		}
		// Strafe right
		if (Input::KeyDown('d')) {
			rb->AddForce(right * speed * deltaTime);
		}
		// Strafe left
		if (Input::KeyDown('a')) {
			rb->AddForce(-right * speed * deltaTime);
		}
		if (Input::KeyPressed('e') && Camera::GetLookingAtDistance() <= interactDistance) {
			interactingWithName = Camera::GetLookingAtName();
		}
		if (Input::KeyPressed('r') && !reloading) {
			reloading = true;
			reloadingTime = glfwGetTime();
		}
		
		if (glfwGetTime() - reloadingTime > WeaponManager::GetGunByName(gunName)->reloadtime && reloading)
		{
			reloading = false;
			WeaponManager::GetGunByName(gunName)->currentammo = WeaponManager::GetGunByName(gunName)->ammo;
			WeaponManager::GetGunByName(gunName)->rotation = 0;
			WeaponManager::GetGunByName(gunName)->down = 1;
		}
		
		//get ray details
		if (Input::LeftMousePressed() && Camera::GetLookingAtDistance() < 9999 && WeaponManager::GetGunByName(gunName)->type == Semi && glfwGetTime() - WeaponManager::GetGunByName(gunName)->lastTimeShot > 60.0f / WeaponManager::GetGunByName(gunName)->firerate && !reloading) {
			if (WeaponManager::GetGunByName(gunName)->currentammo > 0)
			{
				WeaponManager::GetGunByName(gunName)->currentammo--;
				WeaponManager::GetGunByName(gunName)->Shoot();
				verticalAngle += WeaponManager::GetGunByName(gunName)->recoil;
				horizontalAngle += (((double)rand()) / RAND_MAX) / WeaponManager::GetGunByName(gunName)->recoilY;
				WeaponManager::GetGunByName(gunName)->lastTimeShot = glfwGetTime();
				float distance = Camera::GetLookingAtDistance() - 0.015;
				if(Camera::GetLookingAtCollider()->GetStatic())
					AssetManager::AddDecal(Camera::GetRay().origin + distance * Camera::GetRay().direction, Camera::GetNormalFace(), glm::vec3(0.03, 0.03, 0.03), AssetManager::GetTexture("bullet_hole"));
			}
			else {
				//click click
			}
		}
		if (Input::LeftMouseDown() && Camera::GetLookingAtDistance() < 9999  && WeaponManager::GetGunByName(gunName)->type == Auto && glfwGetTime() - WeaponManager::GetGunByName(gunName)->lastTimeShot > 60.0f / WeaponManager::GetGunByName(gunName)->firerate && !reloading) {
			if (WeaponManager::GetGunByName(gunName)->currentammo > 0)
			{
				WeaponManager::GetGunByName(gunName)->currentammo--;
				WeaponManager::GetGunByName(gunName)->Shoot();
				verticalAngle += WeaponManager::GetGunByName(gunName)->recoil;
				horizontalAngle += (((double)rand()) / RAND_MAX) / WeaponManager::GetGunByName(gunName)->recoilY;
				WeaponManager::GetGunByName(gunName)->lastTimeShot = glfwGetTime();
				float distance = Camera::GetLookingAtDistance() - 0.015;
				if (Camera::GetLookingAtCollider()->GetStatic())
					AssetManager::AddDecal(Camera::GetRay().origin + distance * Camera::GetRay().direction, Camera::GetNormalFace(), glm::vec3(0.03, 0.03, 0.03), AssetManager::GetTexture("bullet_hole"));
			}
			else {
				//click click
			}
		}

		horizontalAngle += mouseSpeed * float(1024 / 2 - Input::GetMouseX());

		Camera::SetHorizontalAngle(horizontalAngle);
		Camera::SetVerticalAngle(verticalAngle);
		Camera::SetPosition(rb->GetPostion());
		collider->setPosition(rb->GetPostion()); 
		AssetManager::GetGameObject(WeaponManager::GetGunByName(gunName)->gunModel)->SetRotationX(-verticalAngle);
		AssetManager::GetGameObject(WeaponManager::GetGunByName(gunName)->gunModel)->SetRotationY(horizontalAngle);
		//AssetManager::GetGameObject(WeaponManager::GetGunByName(gunName)->gunModel)->setPosition(glm::vec3(rb->GetPostion().x, rb->GetPostion().y, rb->GetPostion().z));
		if (reloading) {
			WeaponManager::GetGunByName(gunName)->ReloadingAnimation(deltaTime);
		}
		WeaponManager::GetGunByName(gunName)->Update(deltaTime,rb->GetPostion());

	}
	glm::vec3 Player::getPosition() {
		return rb->GetPostion();
	}
	void Player::setPosition(glm::vec3 pos) {
		rb->SetPostion(pos);
		collider->setPosition(pos);
		Camera::SetPosition(rb->GetPostion());
	}
	std::string Player::GetInteractingWithName() {
		return interactingWithName;
	}
	std::string Player::getCurrentGun() {
		return gunName;
	}
	void Player::SelectWeapon(std::string weaponName) {
		if (reloading)
			return;
		AssetManager::GetGameObject(WeaponManager::GetGunByName(gunName)->gunModel)->SetRender(false);
		gunName = weaponName;
		AssetManager::GetGameObject(WeaponManager::GetGunByName(gunName)->gunModel)->SetRender(true);

	}

}