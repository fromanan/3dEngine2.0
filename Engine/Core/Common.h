#pragma once

#include "bullet/btBulletDynamicsCommon.h"

#define SCREENWIDTH 1024
#define SCREENHEIGHT 768
#define WINDOWTITILE "Engine 2.0"

struct Mesh {
    Mesh(const char* path);

    std::vector<unsigned short> indices;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;
    std::vector<glm::vec3> indexed_tangents;
    std::vector<glm::vec3> indexed_bitangents;
};

class Model {
public:
    Model() = default;
    Model(Mesh mesh, Texture* texture);
    void AddMesh(Mesh mesh);
    void SetMesh(int mesh);
    Mesh* GetCurrentMesh();
    const char* GetTextureName();
    void RenderModel(GLuint& programID);

private:
    std::vector<Mesh> meshes;
    int currentMesh = 0;
    Texture* texture = nullptr;

    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint normalbuffer;
    GLuint elementbuffer;
    GLuint tangentbuffer;
    GLuint bitangentbuffer;
};

btVector3 glmToBtVector3(const glm::vec3& vec);

glm::vec3 btToGlmVector3(const btVector3& vec);

glm::vec3 btQuatToGLMVec(const btQuaternion& quat);

glm::mat4 btTransformToGlmMatrix(const btTransform& transform);

glm::mat4 btScalar2mat4(const btScalar* matrix);

glm::mat4 worldToLocal(btVector3 position, btVector3 rotation);

// TODO: I do want to add a mesh instead of storing all the obj info in GameObject this will also make it easy to allow multiple GameObjects to use the same mesh without loading a model each time
