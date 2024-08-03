#pragma once

class Texture
{
public:
    static int CurrentTextureNumber;

    Texture(const char* name, const char* path);
    Texture(const char* name, const char* path, const char* normalPath);

    const char* GetName() const;
    int GetTextureNumber() const;
    int GetTextureNormalNumber() const;

    GLuint GetTexture() const;
    GLuint GetTextureNormal() const;

private:
    const char* name;
    int textureNumber; 
    int textureNormalNumber;
    GLuint texture;
    GLuint textureNormal;
};
