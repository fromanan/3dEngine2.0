#include "Camera.h"


glm::mat4 Camera::getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 Camera::getProjectionMatrix() {
	return ProjectionMatrix;
}