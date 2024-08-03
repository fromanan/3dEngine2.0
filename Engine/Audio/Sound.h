#pragma once
#include <fmod.hpp>

class Sound {
public:
    Sound(const char* Path, std::string Name, glm::vec3 Position, float distance, float Volume);

    FMOD_VECTOR* GetPositionFmod();
    void SetPosition(glm::vec3 Position);
    FMOD::Sound* GetSound() const;
    std::string GetName();
	
    void SetVolume(float Volume);
    float GetVolume() const;

    static constexpr int DEFAULT = -1;
private:
    std::string name;
    FMOD_VECTOR position;
    FMOD::Sound* sound;
    float volume;
};
