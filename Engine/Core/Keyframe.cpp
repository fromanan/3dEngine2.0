#include "pch.h"

#include "Keyframe.h"

void KeyFrame::Create(const glm::vec3 position, const glm::vec3 rotation, const float duration) {
	this->position = position;
	this->rotation = rotation;
	this->duration = duration;
}