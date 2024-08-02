#pragma once
#include <GL/glew.h>

class Texture
{
public:
    static int CurrentTextureNumber;

    Texture(const char* name, const char* path);
    Texture(const char* name, const char* path, const char* normalPath);

    const char* GetName();
    int GetTextureNumber();
    int GetTextureNormalNumber();

    GLuint GetTexture();
    GLuint GetTextureNormal();

private:
    const char* name;
    int textureNumber; 
    int textureNormalNumber;
    GLuint texture;
    GLuint textureNormal;
};
