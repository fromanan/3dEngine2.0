#include "Camera.h"
#include "AssetManager.h"


namespace Camera {

	glm::vec3 position = glm::vec3(0, 0, 5);
	glm::vec3 direction = glm::vec3(0, 0, 0);

	// horizontal angle : toward -Z
	float horizontalAngle = 3.14f;
	float verticalAngle = 0.0f;
	float initialFoV = 45.0f;
	float maxAngle = 1.5;
	float mouseSpeed = 0.005f;

	//ray
	//std::string lookingAtName = "Nothing";
	//glm::vec3 normalFace= glm::vec3(0, 0, 0);
	//Cube* LookingAtcollider = nullptr;
	//float distance = 9999;

	Ray ray;
	RayInfo currentRayInfo;
	RayInfo currentRayInfo2;

	
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	glm::vec3 Camera::GetPostion() {
		return position;
	}
	glm::mat4 Camera::getViewMatrix() {
		return ViewMatrix;
	}
	glm::mat4 Camera::getProjectionMatrix() {
		return ProjectionMatrix;
	}
	void Camera::SetHorizontalAngle(float angle) {
		horizontalAngle = angle;
	}
	void Camera::SetVerticalAngle(float angle) {
		verticalAngle = angle;
	}
	void Camera::SetPosition(glm::vec3 pos) {
		position = pos;
	}
	std::string Camera::GetLookingAtName() {
		return currentRayInfo.name;
	}
	float GetLookingAtDistance() {
		if(currentRayInfo.name != "Nothing")
			return currentRayInfo.distance;
		return -1;
	}
	glm::vec3 Camera::GetRayDirection() {
		return ray.direction;
	}
	Ray Camera::GetRay() {
		return ray;
	}
	glm::vec3 Camera::GetNormalFace() {
		return currentRayInfo.normal;
	}
	Cube* Camera::GetLookingAtCollider() {
		return currentRayInfo.collider;
	}

	RayInfo* GetRayInfo() {
		return &currentRayInfo;
	}
	RayInfo* GetRayInfo2() {
		return &currentRayInfo2;
	}



	void Camera::CheckIntersectingWithRay(Cube* cube) {

		float objectDistance = cube->intersect(ray, 0, 100);
		if (objectDistance > 0 && objectDistance < currentRayInfo.distance)
		{
			currentRayInfo2.collider = currentRayInfo.collider;
			currentRayInfo2.distance = currentRayInfo.distance;
			currentRayInfo2.name = currentRayInfo.name;
			currentRayInfo2.position = currentRayInfo.position;
			currentRayInfo2.normal = currentRayInfo.normal;

			currentRayInfo.collider = cube;
			currentRayInfo.distance = objectDistance;
			currentRayInfo.name = cube->GetName();
			currentRayInfo.position = ray.origin + currentRayInfo.distance * ray.direction;

			// Determine which face was hit by checking the intersection point
			if (fabs(currentRayInfo.position.x - cube->getMin().x) < 0.001f) currentRayInfo.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
			if (fabs(currentRayInfo.position.x - cube->getMax().x) < 0.001f) currentRayInfo.normal = glm::vec3(1.0f, 0.0f, 0.0f);
			if (fabs(currentRayInfo.position.y - cube->getMin().y) < 0.001f) currentRayInfo.normal = glm::vec3(0.0f, -1.0f, 0.0f);
			if (fabs(currentRayInfo.position.y - cube->getMax().y) < 0.001f) currentRayInfo.normal = glm::vec3(0.0f, 1.0f, 0.0f);
			if (fabs(currentRayInfo.position.z - cube->getMin().z) < 0.001f) currentRayInfo.normal = glm::vec3(0.0f, 0.0f, -1.0f);
			if (fabs(currentRayInfo.position.z - cube->getMax().z) < 0.001f) currentRayInfo.normal = glm::vec3(0.0f, 0.0f, 1.0f);
		}

	}

	void Camera::Update(float dt) {
		position.y += 0.75;
		currentRayInfo.distance = 9999;
		currentRayInfo.name = "Nothing";

		currentRayInfo2.distance = 9999;
		currentRayInfo2.name = "Nothing";

		if (verticalAngle <= maxAngle && verticalAngle >= -maxAngle)
			verticalAngle += mouseSpeed * float(768 / 2 - Input::GetMouseY());

		else if (verticalAngle > maxAngle)
			verticalAngle = maxAngle;

		else if (verticalAngle < -maxAngle)
			verticalAngle = -maxAngle;

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);
		// Right vector
		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);
		// Up vector
		glm::vec3 up = glm::cross(right, direction);

		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		ProjectionMatrix = glm::perspective(initialFoV, 4.0f / 3.0f, 0.1f, 100.0f);
		// Camera matrix
		ViewMatrix = glm::lookAt(
			position,           // Camera is here
			position + direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
		);

		ray.UpdateRay(direction, position);

	}
	glm::vec3 Camera::GetDirection() {
		return direction;
	}

}