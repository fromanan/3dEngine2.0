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

#include "Sound.h"

namespace AudioManager {
	void Init();
	void CleanUp();

	void AddSound(const char* path, const std::string& name, const glm::vec3 position, float distance, float volume);
	Sound* GetSound(const std::string& name);
	void Update();
	void UpdateListener(glm::vec3 position, glm::vec3 Forward, glm::vec3 Velocity);
	FMOD::System* GetSystem();

	int PlaySound(std::string sound);
	int PlaySound(const std::string& sound,glm::vec3 Position);
	int PlaySound(const std::string& sound, glm::vec3 Position, int channel);

	int PlaySound(const std::string& sound, int channel);

	bool IsChannelPlaying(int channel);

	void PauseSound(const std::string& sound);
	void StopSound(const std::string& sound);
}
