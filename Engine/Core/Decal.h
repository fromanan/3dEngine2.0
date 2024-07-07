#pragma once
#include "Engine/Core/Common.h"
#include "Engine/Core/Renderer.h"
#include <vector>
#include "string"
class Decal
{
public:
    Decal(glm::vec3 position, glm::vec3 normal, glm::vec3 scale, Texture* texture);
    glm::mat4 GetModel();

    void RenderDecal(GLuint& programID);
private:    
    Texture* texture = NULL;


    glm::mat4 modelMatrix;
    std::vector<unsigned short> indices = { 0, 1, 2, 0, 3, 1 };
    std::vector<glm::vec3> indexed_vertices = { glm::vec3(1.0, 0.0,1.0),glm::vec3(-1.0,0.0,-1.0),glm::vec3(-1.0, 0.0,1.0),glm::vec3(1.0,0.0,-1.0) };
    std::vector<glm::vec2> indexed_uvs = { glm::vec2(1.0,-0.0),glm::vec2(0.0,-1.0),glm::vec2(0.0,-0.0),glm::vec2(1.0,-1.0) };
    std::vector<glm::vec3> indexed_normals = { glm::vec3(-0.0,1.0,-0.0),glm::vec3(-0.0,1.0,-0.0),glm::vec3(-0.0,1.0,-0.0),glm::vec3(-0.0,1.0,-0.0) };

    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint normalbuffer;
    GLuint elementbuffer;
    GLuint VertexArrayID;
    
       

};

