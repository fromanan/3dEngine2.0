#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 colour, float constant, float linear, float quadratic) {
	this->position = position;
	this->colour = colour;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}
