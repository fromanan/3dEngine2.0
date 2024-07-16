#include "Audio.h"


Sound::Sound(const char* Path, std::string Name, glm::vec3 Position, float Distance) {
	position.x = Position.x;
	position.y = Position.y;
	position.z = Position.z;
	FMOD_RESULT result;
	name = Name;
	result = AudioManager::GetSystem()->createSound(Path, FMOD_3D, 0, &sound);
	if (result != FMOD_OK) {
		std::cerr << result << " " << FMOD_ErrorString(result) << std::endl;
	}
	result = sound->set3DMinMaxDistance(0.5f, Distance);
	if (result != FMOD_OK) {
		std::cerr << result << " " << FMOD_ErrorString(result) << std::endl;
	}



}
FMOD::Sound* Sound::GetSound() {
	return sound;
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


namespace AudioManager {
	std::vector<Sound> sounds;
	std::vector<FMOD::Channel*> channels;
	int channelSize = 20;

	const float DISTANCEFACTOR = 1.0f;

	//Listener
	FMOD_VECTOR listenerpos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };


	FMOD_RESULT result;
	FMOD::System* system = nullptr;
	FMOD::ChannelGroup* channelGroup = nullptr;

	bool succeededOrWarn(const std::string& message, FMOD_RESULT result)
	{
		if (result != FMOD_OK) {
			std::cerr << message << ": " << result << " " << FMOD_ErrorString(result) << std::endl;
			return false;
		}
		return true;
	}


	void AudioManager::Init() {
		for (int i = 0; i < channelSize; i++) {
			channels.push_back(0);
		}

		result = FMOD::System_Create(&system);
		if (!succeededOrWarn("FMOD: Failed to create system object", result))
			return;
		result = system->init(512, FMOD_INIT_NORMAL, nullptr);
		if (!succeededOrWarn("FMOD: Failed to create system object", result))
			return;
		result = system->set3DSettings(1.0, DISTANCEFACTOR, 1.0f);
		if (!succeededOrWarn("FMOD: Failed to create system object", result))
			return;
		result = system->createChannelGroup("channels", &channelGroup);
		if (!succeededOrWarn("FMOD: Failed to create system object", result))
			return;

		FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
		FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };


		
	}

	void AudioManager::CleanUp() {
		channelGroup->release();
		system->release();
	}
	FMOD::System* AudioManager::GetSystem() {
		return system;
	}

	void AudioManager::AddSound(const char* Path, std::string Name, glm::vec3 Position, float distance) {
		sounds.push_back(Sound(Path,Name,Position,distance));
	}
	Sound* AudioManager::GetSound(std::string name) {
		for (int i = 0; i < sounds.size(); i++) {
			if (sounds[i].GetName() == name)
				return &sounds[i];
		}
	}
	void UpdateListener(glm::vec3 Position, glm::vec3 Forward,glm::vec3 Velocity) {
		listenerpos.x = Position.x;
		listenerpos.y = Position.y;
		listenerpos.z = Position.z;

		forward.x = Forward.x;
		forward.y = Forward.y;
		forward.z = Forward.z;

		vel.x = Velocity.x;
		vel.y = Velocity.y;
		vel.z = Velocity.z;

		result = system->set3DListenerAttributes(0, &listenerpos, &vel, &forward, &up);
		if (!succeededOrWarn("FMOD: Failed to create system object", result))
			return;

	}
	void AudioManager::Update() {
		system->update();
	}

	void AudioManager::PlaySound(std::string sound) {
		bool isPlaying = false;
		std::cout << "Trying to Play " << sound << std::endl;

		for (int i = 0; i < channelSize; i++) {
			channels[i]->isPlaying(&isPlaying);
			if (!isPlaying) {
				std::cout << "Playing " << sound << std::endl;
				result = system->playSound(GetSound(sound)->GetSound(), 0, true, &channels[i]);
				result = channels[i]->set3DAttributes(GetSound(sound)->GetPositionFmod(), &vel);
				result = channels[i]->setPaused(false);
				if (!succeededOrWarn("FMOD: Failed to create system object", result))
					return;
				break;
			}
		}		
	}
	void AudioManager::PlaySound(std::string sound, int channel) {
		std::cout << "Playing " << sound << std::endl;
		result = system->playSound(GetSound(sound)->GetSound(), 0, true, &channels[channel]);
		result = channels[channel]->set3DAttributes(GetSound(sound)->GetPositionFmod(), &vel);
		result = channels[channel]->setPaused(false);
		if (!succeededOrWarn("FMOD: Failed to create system object", result))
			return;
	}

	void AudioManager::PauseSound(std::string sound) {

	}
	void AudioManager::StopSound(std::string sound) {

	}
}