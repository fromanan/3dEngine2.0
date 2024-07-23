#pragma once
#include "Common.h"



btVector3 glmToBtVector3(const glm::vec3& vec) {
    return btVector3(vec.x, vec.y, vec.z);
}
glm::vec3 btToGlmVector3(const btVector3& vec) {
    return glm::vec3(vec.getX(), vec.getY(), vec.getZ());
}

glm::vec3 btQuatToGLMVec(const btQuaternion& quat) {
    // Convert btQuaternion to glm::quat
    glm::quat glmQuat(quat.getW(), quat.getX(), quat.getY(), quat.getZ());

    // Convert glm::quat to glm::mat4
    glm::mat4 rotationMatrix = glm::mat4_cast(glmQuat);

    // Extract Euler angles from glm::mat4
    glm::vec3 eulerAngles = glm::eulerAngles(glmQuat);

    return eulerAngles;
}


