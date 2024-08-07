#pragma once

class GunPickup
{
public:
    GunPickup(const std::string& gunName, const std::string& objectName,  Model* model, glm::vec3 position);
    GunPickup(const std::string& gunName, const std::string& gunObject, glm::vec3 position);
    void Update();
    bool Interact() const;
private:
    std::string gunName;
    std::string objectName;
};
