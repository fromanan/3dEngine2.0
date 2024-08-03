#pragma once

namespace Camera
{
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	glm::vec3 GetPosition();
	void SetHorizontalAngle(float angle);
	void SetVerticalAngle(float angle);
	void SetPosition(glm::vec3 pos);
	glm::vec3 GetDirection();
	void Update(float dt);

	glm::vec3 ComputeRay();
	btCollisionWorld::ClosestRayResultCallback GetRayHit();
}
