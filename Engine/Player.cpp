#include "Player.h"
#include "AssetManager.h"

namespace Player
{
	float horizontalAngle = 3.14f;
	float verticalAngle = 0.0f;
	float initialFoV = 45.0f;
	float maxAngle = 1.5;
	float mouseSpeed = 0.005f;
	float speed = 35;
	float jumpforce = 50000;
	RigidBody* rb;
	Cube* collider;

	GameObject* playerModel;
	GameObject* gun;

	void Player::Init() {
		rb = PhysicsManager::AddRigidbody(glm::vec3(0, 0, 5), "PlayerRB");
		collider = PhysicsManager::AddCube(rb->GetPostion(), 0.5, 2, 0.5, "PlayerCollider");
		rb->SetColider(collider);
		std::cout << "loading player model" << std::endl;
		//gun = AssetManager::GetGameObject(AssetManager::AddGameObject("playerGun", "Assets/Objects/gun.obj", AssetManager::GetTexture("uvmap"), glm::vec3(1,0.8,0.2)));
		//gun->SetParentName("player");

		playerModel = AssetManager::GetGameObject(AssetManager::AddGameObject("player", "Assets/Objects/capsule.obj", AssetManager::GetTexture("uvmap"), glm::vec3(rb->GetPostion().x, rb->GetPostion().y - 1.25, rb->GetPostion().z)));

	}

	void Player::Update(float deltaTime) {

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

		horizontalAngle += mouseSpeed * float(1024 / 2 - Input::GetMouseX());

		Camera::SetHorizontalAngle(horizontalAngle);
		Camera::SetVerticalAngle(verticalAngle);
		Camera::SetPosition(rb->GetPostion());
		collider->setPosition(rb->GetPostion());
		playerModel->setPosition(glm::vec3(rb->GetPostion().x, rb->GetPostion().y - 1.25, rb->GetPostion().z));
		playerModel->SetRotationY(horizontalAngle);

	}
	glm::vec3 Player::getPosition() {
		return rb->GetPostion();
	}
	void Player::setPosition(glm::vec3 pos) {
		rb->SetPostion(pos);
		collider->setPosition(pos);
		Camera::SetPosition(rb->GetPostion());
	}



}