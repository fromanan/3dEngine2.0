#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Skybox.h"
#include "Texture.h"
#include "Loaders/ShaderLoader.hpp"
#include "Loaders/Loader.hpp"
#include "Engine/Core/UI/Text2D.h"
#include "Loaders/stb_image.h"
#include "Engine/Core/Common.h"
#include "Engine/Core/Lights/Light.h"

namespace Renderer
{
    int init(const char* vertex, const char* fragment, const char* name);
    int LoadShader(const char* vertex, const char* fragment, const char* name);

    void ClearScreen();
    void UseProgram(GLuint ProgramID);
    void SwapBuffers(GLFWwindow* window);

    GLuint GetCurrentProgramID();
    GLuint GetProgramID(const char* name);

    // Text Rendering
    void RenderText(const char* text, int x, int y, int size);
    void DrawSprite(Texture* texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    void RendererSkybox(glm::mat4 view, glm::mat4 projection, Skybox skybox);

    // Shader
    void SetTextureShader(glm::mat4 mvp, glm::mat4 model, glm::mat4 view, glm::mat3 ModelView3x3Matrix);
    void SetLights(std::vector<Light> lights);

    void setMat4(GLuint id, glm::mat4& mat4);
    void setVec3(GLuint id, glm::vec3& vec3);
}
