#pragma once

#include "Tags.h"

class RigidBody
{
public:
    RigidBody();
    RigidBody(glm::vec3 position, const std::string& name);
    glm::vec3 GetPosition() const;
    void SetPosition(glm::vec3 position);
    void SetCollider(const std::string& colliderName);
    std::string GetCollider();

    std::string GetName();

    void NewPosition(float deltaTime);
    void NewPositionY(float deltaTime);

    void AddForce(glm::vec3 force);
    void AddForceX(float force);
    void AddForceY(float force);
    void AddForceZ(float force);

    void SetForce(glm::vec3 force);
    void SetForceX(float force);
    void SetForceY(float force);
    void SetForceZ(float force);

    void RemoveForceX();
    void RemoveForceY();
    void RemoveForceZ();

    glm::vec3 GetForce();

private:
    std::string name = Tags::NONE;
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 velocity = glm::vec3(0,0,0);
    std::string collider = Tags::NONE;
};
