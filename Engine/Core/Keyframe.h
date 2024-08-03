#pragma once

struct KeyFrame
{
    float duration;
    glm::vec3 position;
    glm::vec3 rotation;

    void Create(glm::vec3 position, glm::vec3 rotation, float duration);
};