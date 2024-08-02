﻿#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Ray
{
public:
    glm::vec3 direction;
    glm::vec3 origin;
    Ray();
    Ray(glm::vec3 dir, glm::vec3 org);
    void UpdateRay(glm::vec3 dir, glm::vec3 org);
    bool intersectsTriangle(std::vector<glm::vec3> vertices, glm::mat4 ModelMatrix) const;
};