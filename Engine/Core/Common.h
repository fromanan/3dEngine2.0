#pragma once

#include "bullet/btBulletDynamicsCommon.h"

#define SCREENWIDTH 1024
#define SCREENHEIGHT 768
#define WINDOWTITILE "Engine 2.0"

btVector3 glmToBtVector3(const glm::vec3& vec);

glm::vec3 btToGlmVector3(const btVector3& vec);

glm::vec3 btQuatToGLMVec(const btQuaternion& quat);

glm::mat4 btTransformToGlmMatrix(const btTransform& transform);

glm::mat4 btScalar2mat4(const btScalar* matrix);

glm::mat4 worldToLocal(btVector3 position, btVector3 rotation);

// TODO: I do want to add a mesh instead of storing all the obj info in GameObject this will also make it easy to allow multiple GameObjects to use the same mesh without loading a model each time
