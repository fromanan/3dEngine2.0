#include "Player.h"

namespace Player
{
	float horizontalAngle = 3.14f;
	float verticalAngle = 0.0f;
	float initialFoV = 45.0f;
	float maxAngle = 1.5;
	float mouseSpeed = 0.005f;
	float speed = 20;
	RigidBody* rb;
	Cube* collider;

	void Player::Init() {
		rb = PhysicsManager::AddRigidbody(glm::vec3(0, 0, 1), "PlayerRB");
		collider = PhysicsManager::AddCube(glm::vec3(0, 0, 1), 0.2, 1, 0.2, "PlayerCollider");
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
	}
	glm::vec3 Player::getPosition() {
		return rb->GetPostion();
	}
	void Player::setPosition(glm::vec3 pos) {
		rb->SetPostion(pos);
		Camera::SetPosition(rb->GetPostion());
	}



}