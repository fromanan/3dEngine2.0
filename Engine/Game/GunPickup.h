#pragma once

class GunPickup
{
public:
    GunPickup(const std::string& gunName, const std::string& objectName, const char* objectModel, Texture* texture, glm::vec3 position);
    GunPickup(const std::string& gun_name, const std::string& gunObject, glm::vec3 position);
    void Update();
    bool Interact() const;
private:
    std::string gunName;
    std::string objectName;
};
