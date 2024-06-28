#include "Camera.h"


glm::mat4 Camera::getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 Camera::getProjectionMatrix() {
	return ProjectionMatrix;
}

void Camera::Update(float dt) {
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

	horizontalAngle += mouseSpeed * float(1024 / 2 - Input::GetMouseX());

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	//std::cout << " X: " << direction.x << " Y: " << direction.y << " Z: " << direction.z << std::endl;
	//forward vector
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
	// Up vector
	glm::vec3 up = glm::cross(right, direction);
	offset = glm::vec3(0, offset.y, 0);

	// Move forward
	if (Input::KeyDown('w')) {
		offset -= forward * 100.0f * dt;
	}
	// Move backward
	if (Input::KeyDown('s')) {
		offset += forward * 100.0f * dt;
	}

	// Strafe right
	if (Input::KeyDown('d')) {
		offset += right * 100.0f * dt;
	}
	// Strafe left
	if (Input::KeyDown('a')) {
		offset -= right * 100.0f * dt;
	}

	position += offset;

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(initialFoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

}
