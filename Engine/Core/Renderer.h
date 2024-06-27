#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>





class Texture {
public:
    static int CurrentTextureNumber;

    Texture(const char* name, const char* path);

    const char* GetName();
    int GetTextureNumber();
    GLuint GetTexture();
private:
    const char* name;
    int textureNumber;
    GLuint texture;
};