#pragma once

class Skybox
{
public:
    Skybox();
    Skybox(std::vector<std::string> faces);
    unsigned int GetTextureID() const;
    unsigned int GetSkyboxVAO();
private:
    unsigned int textureID;
    unsigned int skyboxVAO, skyboxVBO;
    static float skyboxVertices[108];
};
