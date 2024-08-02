#include "Sound.h"

#include "AudioManager.h"

Sound::Sound(const char* Path, std::string Name, glm::vec3 Position, float Distance, float Volume) {
    position.x = Position.x;
    position.y = Position.y;
    position.z = Position.z;
    name = Name;
    FMOD_RESULT result = AudioManager::GetSystem()->createSound(Path, FMOD_3D, 0, &sound);
    if (result != FMOD_OK) {
        std::cerr << result << " " << FMOD_ErrorString(result) << std::endl;
    }
    result = sound->set3DMinMaxDistance(0.5f, Distance);
    if (result != FMOD_OK) {
        std::cerr << result << " " << FMOD_ErrorString(result) << std::endl;
    }
    volume = Volume;
}

FMOD::Sound* Sound::GetSound() const {
    return sound;
}

void Sound::SetVolume(float Volume) {
    volume = Volume;
}

float Sound::GetVolume() const {
    return volume;
}

FMOD_VECTOR* Sound::GetPositionFmod() {
    return &position;
}

void Sound::SetPosition(glm::vec3 Position) {
    position.x = Position.x;
    position.y = Position.y;
    position.z = Position.z;
}

std::string Sound::GetName() {
    return name;
}
