#include "Common.h"
#include "Engine/Loaders/Loader.hpp"
#include "Engine/Loaders/vboindexer.h"


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

glm::mat4 btTransformToGlmMatrix(const btTransform& transform) {
    // Extract position and rotation from Bullet transform
    btVector3 position = transform.getOrigin();
    btQuaternion rotation = transform.getRotation();

    // Convert Bullet quaternion to glm::quat
    glm::quat glmRotation(rotation.w(), rotation.x(), rotation.y(), rotation.z());

    // Create glm::mat4 from position and rotation
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x(), position.y(), position.z()));
    model = model * glm::mat4_cast(glmRotation);

    return model;
}

glm::mat4 btScalar2mat4(btScalar* matrix) {
    return glm::mat4(
        matrix[0], matrix[1], matrix[2], matrix[3],
        matrix[4], matrix[5], matrix[6], matrix[7],
        matrix[8], matrix[9], matrix[10], matrix[11],
        matrix[12], matrix[13], matrix[14], matrix[15]);
}

glm::mat4 worldToLocal(btVector3 position, btVector3 rotation) {
    glm::mat4 m = glm::translate(glm::mat4(1), btToGlmVector3(position));
    m *= glm::mat4_cast(glm::quat(btToGlmVector3(rotation)));
    m = glm::scale(m, glm::vec3(1, 1, 1));
    return m;
}



Mesh::Mesh(const char* path) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    bool res = Loader::loadOBJ(path, vertices, uvs, normals);
    //compute tangents
    for (int i = 0; i < vertices.size(); i += 3) {
        // Shortcuts for vertices
        glm::vec3& v0 = vertices[i + 0];
        glm::vec3& v1 = vertices[i + 1];
        glm::vec3& v2 = vertices[i + 2];

        // Shortcuts for UVs
        glm::vec2& uv0 = uvs[i + 0];
        glm::vec2& uv1 = uvs[i + 1];
        glm::vec2& uv2 = uvs[i + 2];
        // Edges of the triangle : position delta
        glm::vec3 deltaPos1 = v1 - v0;
        glm::vec3 deltaPos2 = v2 - v0;

        // UV delta
        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        // Set the same tangent for all three vertices of the triangle.
        // They will be merged later, in vboindexer.cpp
        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        // Same thing for bitangents
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }

    indexer::indexVBO(vertices, uvs, normals, tangents, bitangents, indices, indexed_vertices, indexed_uvs, indexed_normals, indexed_tangents, indexed_bitangents);

}


Model::Model(Mesh mesh, Texture* texture) {
    this->texture = texture;
    meshes.push_back(mesh);

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, meshes[0].indexed_vertices.size() * sizeof(glm::vec3), &meshes[0].indexed_vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, meshes[0].indexed_uvs.size() * sizeof(glm::vec2), &meshes[0].indexed_uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, meshes[0].indexed_normals.size() * sizeof(glm::vec3), &meshes[0].indexed_normals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshes[0].indices.size() * sizeof(unsigned short), &meshes[0].indices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &tangentbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
    glBufferData(GL_ARRAY_BUFFER, meshes[0].indexed_tangents.size() * sizeof(glm::vec3), &meshes[0].indexed_tangents[0], GL_STATIC_DRAW);

    glGenBuffers(1, &bitangentbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
    glBufferData(GL_ARRAY_BUFFER, meshes[0].indexed_bitangents.size() * sizeof(glm::vec3), &meshes[0].indexed_bitangents[0], GL_STATIC_DRAW);

}
void Model::AddMesh(Mesh mesh) {
    meshes.push_back(mesh);
}
void Model::SetMesh(int mesh) {
    if(mesh >= 0 && mesh < meshes.size())
        currentMesh = mesh;
}
void Model::RenderModel(GLuint& programID) {

    glUseProgram(programID);

    if (texture != nullptr) {
        glActiveTexture(texture->GetTextureNumber() + GL_TEXTURE0);
        GLuint TextureID = glGetUniformLocation(programID, "DiffuseTextureSampler");
        glBindTexture(GL_TEXTURE_2D, texture->GetTexture());
        glUniform1i(TextureID, texture->GetTextureNumber());

        glActiveTexture(texture->GetTextureNormalNumber() + GL_TEXTURE0);
        GLuint NormalID = glGetUniformLocation(programID, "NormalTextureSampler");
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureNormal());
        glUniform1i(NormalID, texture->GetTextureNormalNumber());
    }

    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
        1,                                // attribute
        2,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
        2,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // 4th attribute buffer : tangents
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
    glVertexAttribPointer(
        3,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // 5th attribute buffer : bitangents
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
    glVertexAttribPointer(
        4,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    // Draw the triangles !
    glDrawElements(
        GL_TRIANGLES,      // mode
        (GLsizei)meshes[currentMesh].indices.size(),    // count
        GL_UNSIGNED_SHORT,   // type
        (void*)0           // element array buffer offset
    );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
}

