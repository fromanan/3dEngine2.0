#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <fmod.hpp>
#include <fmod_errors.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Sound {
public:
	Sound(const char* Path, std::string Name, glm::vec3 Position, float distance);

	FMOD_VECTOR* GetPositionFmod();
	void SetPosition(glm::vec3 Position);
	FMOD::Sound* GetSound();
	std::string GetName();

private:
	std::string name;
	FMOD_VECTOR position;
	FMOD::Sound* sound;

};



namespace AudioManager {
	void Init();
	void CleanUp();

	void AddSound(const char* Path, std::string Name, glm::vec3 Position, float distance);
	Sound* GetSound(std::string name);
	void Update();
	void UpdateListener(glm::vec3 Position, glm::vec3 Forward, glm::vec3 Velocity);
	FMOD::System* GetSystem();

	void PlaySound(std::string sound);
	void PlaySound(std::string sound, int channel);

	void PauseSound(std::string sound);
	void StopSound(std::string sound);


}