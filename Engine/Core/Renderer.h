#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Loaders/ShaderLoader.hpp"
#include "Loaders/loader.hpp"
#include "Engine/Core/UI/Text2D.h"


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

namespace Renderer {
    int init(const char* vertex, const char* fragment, const char* name);
    int LoadShader(const char* vertex, const char* fragment);

    void ClearScreen();
    void UseProgram(int ProgramID);
    void SwapBuffers(GLFWwindow* window);

    GLuint GetCurrentProgramID();
    GLuint GetProgramID(const char* name);

    //Text Rendering
    void RenderText(const char* text, int x, int y, int size);

    void setMat4(GLuint id, glm::mat4& mat4);
    void setVec3(GLuint id, glm::vec3& vec3);
}