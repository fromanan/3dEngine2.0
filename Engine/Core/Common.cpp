#pragma once
#include "Common.h"



btVector3 glmToBtVector3(const glm::vec3& vec) {
    return btVector3(vec.x, vec.y, vec.z);
}
glm::vec3 btToGlmVector3(const btVector3& vec) {
    return glm::vec3(vec.getX(), vec.getY(), vec.getZ());
}