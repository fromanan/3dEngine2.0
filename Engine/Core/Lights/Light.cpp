#include "pch.h"

#include "Light.h"

Light::Light(const glm::vec3 position, const glm::vec3 color, const float constant, const float linear, const float quadratic) {
	this->position = position;
	this->color = color;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}
