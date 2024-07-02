#include "Player.h"

namespace Player
{
	glm::vec3 position;
	float horizontalAngle = 3.14f;
	float verticalAngle = 0.0f;
	float initialFoV = 45.0f;
	float maxAngle = 1.5;
	float mouseSpeed = 0.005f;

	float speed = 1000;

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
			position -= forward * 10.0f * deltaTime;
		}
		// Move backward
		if (Input::KeyDown('s')) {
			position += forward * 10.0f * deltaTime;
		}

		// Strafe right
		if (Input::KeyDown('d')) {
			position += right * 10.0f * deltaTime;
		}
		// Strafe left
		if (Input::KeyDown('a')) {
			position -= right * 10.0f * deltaTime;
		}
		horizontalAngle += mouseSpeed * float(1024 / 2 - Input::GetMouseX());

		Camera::SetHorizontalAngle(horizontalAngle);
		Camera::SetVerticalAngle(verticalAngle);
		Camera::SetPosition(position);
	}
	glm::vec3 Player::getPosition() {
		return position;
	}
	void Player::setPosition(glm::vec3 pos) {
		position = pos;
	}

}