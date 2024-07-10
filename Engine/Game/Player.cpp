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
	Gun* CurrentGun;
	GameObject* playerModel; 

	std::string interactingWithName = "Nothing";
	float interactDistance = 2;

	//states
	bool reloading = false;
	double reloadingTime = 0;
	

	void Player::Init() {
		srand(time(0));

		rb = PhysicsManager::AddRigidbody(glm::vec3(0, 0, 5), "PlayerRB");
		collider = PhysicsManager::AddCube(rb->GetPostion(), 0.5, 3, 0.5, "PlayerCollider");
		rb->SetColider(collider);
		std::cout << "loading player model" << std::endl;
		playerModel = AssetManager::GetGameObject(AssetManager::AddGameObject("player", "Assets/Objects/capsule.obj", AssetManager::GetTexture("uvmap"), glm::vec3(rb->GetPostion().x, rb->GetPostion().y - 1.25, rb->GetPostion().z)));

		CurrentGun = WeaponManager::GetGunByName("ak47");
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
		
		if (glfwGetTime() - reloadingTime > CurrentGun->reloadtime && reloading)
		{
			reloading = false;
			CurrentGun->currentammo = CurrentGun->ammo;
			CurrentGun->rotation = 0;
			CurrentGun->down = 1;
		}
		
		//get ray details
		if (Input::LeftMousePressed() && Camera::GetLookingAtDistance() < 9999 && Camera::GetLookingAtCollider()->GetStatic() && CurrentGun->type == Semi && glfwGetTime() - CurrentGun->lastTimeShot > 60.0f / CurrentGun->firerate && !reloading) {
			if (CurrentGun->currentammo > 0)
			{
				CurrentGun->currentammo--;
				verticalAngle += CurrentGun->recoil;
				horizontalAngle += (((double)rand()) / RAND_MAX) / CurrentGun->recoilY;
				CurrentGun->lastTimeShot = glfwGetTime();
				float distance = Camera::GetLookingAtDistance() - 0.015;
				AssetManager::AddDecal(Camera::GetRay().origin + distance * Camera::GetRay().direction, Camera::GetNormalFace(), glm::vec3(0.1, 0.1, 0.1), AssetManager::GetTexture("bullet_hole"));
			}
			else {
				//click click
			}
		}
		if (Input::LeftMouseDown() && Camera::GetLookingAtDistance() < 9999 && Camera::GetLookingAtCollider()->GetStatic() && CurrentGun->type == Auto && glfwGetTime() - CurrentGun->lastTimeShot > 60.0f / CurrentGun->firerate && !reloading) {
			if (CurrentGun->currentammo > 0)
			{
				CurrentGun->currentammo--;
				verticalAngle += CurrentGun->recoil;
				horizontalAngle += (((double)rand()) / RAND_MAX) / CurrentGun->recoilY;
				std::cout << "test";
				CurrentGun->lastTimeShot = glfwGetTime();
				float distance = Camera::GetLookingAtDistance() - 0.015;
				AssetManager::AddDecal(Camera::GetRay().origin + distance * Camera::GetRay().direction, Camera::GetNormalFace(), glm::vec3(0.1, 0.1, 0.1), AssetManager::GetTexture("bullet_hole"));
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
		playerModel->setPosition(glm::vec3(rb->GetPostion().x, rb->GetPostion().y - 1.25, rb->GetPostion().z));
		playerModel->SetRotationY(horizontalAngle);
		AssetManager::GetGameObject(CurrentGun->gunModel)->SetRotationX(-verticalAngle);
		AssetManager::GetGameObject(CurrentGun->gunModel)->SetRotationY(horizontalAngle);
		AssetManager::GetGameObject(CurrentGun->gunModel)->setPosition(glm::vec3(rb->GetPostion().x, rb->GetPostion().y, rb->GetPostion().z));
		if (reloading) {
			CurrentGun->ReloadingAnimation(deltaTime);
		}
	}
	glm::vec3 Player::getPosition() {
		return rb->GetPostion();
	}
	void Player::setPosition(glm::vec3 pos) {
		rb->SetPostion(pos);
		collider->setPosition(pos);
		Camera::SetPosition(rb->GetPostion());
	}
	std::string GetInteractingWithName() {
		return interactingWithName;
	}
	Gun* getCurrentGun() {
		return CurrentGun;
	}

}