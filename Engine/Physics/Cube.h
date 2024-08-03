#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GameObject.h"
#include "Ray.h"
#include "Tags.h"

class Cube
{
public:
    Cube(glm::vec3 position, float width, float height, float depth, std::string name);
    Cube(glm::vec3 position, glm::vec3 min, glm::vec3 max, std::string name);

    std::string GetName();
    std::string GetTag();
    void SetTag(const std::string& tag);

    void setPosition(glm::vec3 position);
    glm::vec3 getPosition();
    float getDepth() const;
    float getHeight() const;
    float getWidth() const;
    void setDimensions(float width, float height, float depth);

    glm::vec3 getMin() const;
    glm::vec3 getMax() const;

    bool GetIsTrigger() const;
    void SetIsTrigger(bool trigger);
    void SetStatic(bool isStatic);
    bool GetStatic() const;

    void SetDelete(bool Delete);
    bool ShouldDelete() const;
	
    bool TouchingLeft(const Cube* collider, float velocity) const;
    bool TouchingRight(const Cube* collider, float velocity) const;
    bool TouchingFront(const Cube* collider, float velocity) const;
    bool TouchingBack(const Cube* collider, float velocity) const;
    bool TouchingBottom(const Cube* collider, const float velocity) const;
    bool TouchingTop(const Cube* collider, const float velocity) const;

    float intersect(const Ray& r, float t0, float t1) const;

    Cube(GameObject* gameObject, std::string name);
    void Regenerate(GameObject* gameObject);

private:
    std::string name = Tags::NONE;
    std::string tag = Tags::NONE;
    glm::vec3 min;
    glm::vec3 max;
    glm::vec3 position;
    float width, depth, height;
    bool isTrigger = false;
    bool isStatic = true;
    bool shouldDelete = false;
};
