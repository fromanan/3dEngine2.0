#pragma once

#include "Tags.h"

class Crate
{
public:
	Crate(glm::vec3 position, const std::string& name, Model* model);
	void Update() const;
	void Break() const;
	void DealDamage(int damage, glm::vec3 position, glm::vec3 force);
	std::string GetName();
private:
	std::string name = Tags::NONE;
	int health = 500;
};