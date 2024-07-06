#include "Camera.h"
#include "AssetManager.h"


namespace Camera {

	glm::vec3 position = glm::vec3(0, 0, 5);

	// horizontal angle : toward -Z
	float horizontalAngle = 3.14f;
	float verticalAngle = 0.0f;
	float initialFoV = 45.0f;
	float maxAngle = 1.5;
	float mouseSpeed = 0.005f;

	//ray
	std::string lookingAtName = "Nothing";
	float distance = 9999;
	Ray ray;
	
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
		return lookingAtName;
	}
	float GetLookingAtDistance() {
		if(lookingAtName != "Nothing")
			return distance;
		return -1;
	}

	void Camera::CheckIntersectingWithRay(Cube* cube) {

		float objectDistance = cube->intersect(ray, 0, 100);
		if (objectDistance > 0 && objectDistance < distance)
		{
			
			lookingAtName = cube->GetName();
			distance = objectDistance;
			glm::vec3 intersectionPoint(ray.origin + distance * ray.direction);
			//Makes the decal not overlap with the object
			objectDistance -= 0.01;
			// Determine which face was hit by checking the intersection point
			glm::vec3 normal(0,0,0);
			glm::vec3 up(0,1,0);
			if (fabs(intersectionPoint.x - cube->getMin().x) < 0.001f) normal = glm::vec3(-1.0f, 0.0f, 0.0f);
			if (fabs(intersectionPoint.x - cube->getMax().x) < 0.001f) normal = glm::vec3(1.0f, 0.0f, 0.0f);
			if (fabs(intersectionPoint.y - cube->getMin().y) < 0.001f) normal = glm::vec3(0.0f, -1.0f, 0.0f);
			if (fabs(intersectionPoint.y - cube->getMax().y) < 0.001f) normal = glm::vec3(0.0f, 1.0f, 0.0f);
			if (fabs(intersectionPoint.z - cube->getMin().z) < 0.001f) normal = glm::vec3(0.0f, 0.0f, -1.0f);
			if (fabs(intersectionPoint.z - cube->getMax().z) < 0.001f) normal = glm::vec3(0.0f, 0.0f, 1.0f);

			if (Input::KeyPressed('e'))
			{
				glm::vec3 rotationAxis = glm::cross(normal, up);
				float angle = acos(glm::dot(normal, up));
				int index = AssetManager::AddGameObject("point", "Assets/Objects/Floor.obj", AssetManager::GetTexture("bullet_hole"), ray.origin + objectDistance * ray.direction);
				AssetManager::GetGameObject(index)->SetScale(0.002);
				AssetManager::GetGameObject(index)->setRotation(rotationAxis * -angle);
			}
		}
	}

	void Camera::Update(float dt) {
		distance = 9999;
		lookingAtName = "Nothing";

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		glm::vec3 direction(
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

}