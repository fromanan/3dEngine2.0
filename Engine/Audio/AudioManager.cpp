#include "AudioManager.h"

#include "AssetPaths.h"

namespace AudioManager {
	std::vector<Sound> sounds;
	std::vector<FMOD::Channel*> channels;
	int channelSize = 50;

	constexpr float DISTANCEFACTOR = 1.0f;

	//Listener
	FMOD_VECTOR listenerpos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

	FMOD_RESULT result;
	FMOD::System* system = nullptr;
	FMOD::ChannelGroup* channelGroup = nullptr;

	bool succeededOrWarn(const std::string& message, FMOD_RESULT result) {
		if (result != FMOD_OK) {
			std::cerr << message << ": " << result << " " << FMOD_ErrorString(result) << std::endl;
			return false;
		}
		return true;
	}

	void AudioManager::Init() {
		for (int i = 0; i < channelSize; i++) {
			channels.push_back(nullptr);
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
		
		// Thanks to @livinamuk for a couple of these sounds
		AudioManager::AddSound(AssetPaths::Audio_DoorClose, "door_close", glm::vec3(0, 0, 0), 10, 1.0f);
		AudioManager::AddSound(AssetPaths::Audio_DoorOpen, "door_open", glm::vec3(0, 0, 0), 10, 0.8f);
		AudioManager::AddSound(AssetPaths::Audio_PlayerStep_1, "foot_step1", glm::vec3(0, 0, 0), 10, 0.3f);
		AudioManager::AddSound(AssetPaths::Audio_PlayerStep_2, "foot_step2", glm::vec3(0, 0, 0), 10, 0.3f);
		AudioManager::AddSound(AssetPaths::Audio_PlayerStep_3, "foot_step3", glm::vec3(0, 0, 0), 10, 0.3f);
		AudioManager::AddSound(AssetPaths::Audio_PlayerStep_4, "foot_step4", glm::vec3(0, 0, 0), 10, 0.3f);
		AudioManager::AddSound(AssetPaths::Audio_ItemPickup, "item_pickup", glm::vec3(0, 0, 0), 10, 0.8f);
		AudioManager::AddSound(AssetPaths::Audio_GlassImpact_1, "glass_impact1", glm::vec3(0, 0, 0), 10, 0.8f);
		AudioManager::AddSound(AssetPaths::Audio_GlassImpact_2, "glass_impact2", glm::vec3(0, 0, 0), 10, 0.8f);
	}

	void AudioManager::CleanUp() {
		channelGroup->release();
		system->release();
	}
	
	FMOD::System* AudioManager::GetSystem() {
		return system;
	}

	void AudioManager::AddSound(const char* path, const std::string& name, const glm::vec3 position, float distance, float volume) {
		sounds.emplace_back(path, name, position, distance, volume);
	}
	
	Sound* AudioManager::GetSound(const std::string& name) {
		for (auto& sound : sounds) {
			if (sound.GetName() == name)
				return &sound;
		}
		return nullptr;
	}
	
	void UpdateListener(const glm::vec3 position, const glm::vec3 Forward, const glm::vec3 Velocity) {
		listenerpos.x = position.x;
		listenerpos.y = position.y;
		listenerpos.z = position.z;

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
	
	bool AudioManager::IsChannelPlaying(int channel) {
		if (channel < 0 || channel > channelSize)
			return false;
		bool isPlaying = false;
		channels[channel]->isPlaying(&isPlaying);
		return isPlaying;
	}

	int AudioManager::PlaySound(std::string sound) {
		bool isPlaying = false;
		for (int i = 0; i < channelSize; i++) {
			channels[i]->isPlaying(&isPlaying);
			if (!isPlaying) {
				
				result = system->playSound(GetSound(sound)->GetSound(), 0, true, &channels[i]);
				if (!succeededOrWarn("FMOD2: ", result))
					return Sound::DEFAULT;
				result = channels[i]->set3DAttributes(GetSound(sound)->GetPositionFmod(), &vel);
				if (!succeededOrWarn("FMOD3: ", result))
					return Sound::DEFAULT;
				result = channels[i]->setVolume(GetSound(sound)->GetVolume());
				if (!succeededOrWarn("FMOD1: ", result))
					return Sound::DEFAULT;
				result = channels[i]->setPaused(false);
				if (!succeededOrWarn("FMOD4: ", result))
					return Sound::DEFAULT;

				return i;
			}
		}

		return Sound::DEFAULT;
	}
	
	int PlaySound(const std::string& sound, const glm::vec3 Position) {
		bool isPlaying = false;
		for (int i = 0; i < channelSize; i++) {
			channels[i]->isPlaying(&isPlaying);
			if (!isPlaying) {
				result = system->playSound(GetSound(sound)->GetSound(), 0, true, &channels[i]);
				if (!succeededOrWarn("Error1", result))
					return Sound::DEFAULT;
				//GetSound(sound)->SetPosition(Position);

				FMOD_VECTOR position;
				position.x = Position.x;
				position.y = Position.y;
				position.z = Position.z;

				result = channels[i]->set3DAttributes(&position, &vel);
				if (!succeededOrWarn("Error2", result))
					return Sound::DEFAULT;
				result = channels[i]->setPaused(false);
				if (!succeededOrWarn("Error3", result))
					return Sound::DEFAULT;
				channels[i]->setVolume(GetSound(sound)->GetVolume());
				if (!succeededOrWarn("Error4", result))
					return Sound::DEFAULT;
				
				return i;
			}
		}
		
		return Sound::DEFAULT;
	}
	
	int PlaySound(const std::string& sound, const glm::vec3 Position, const int channel) {
		FMOD_VECTOR position;
		position.x = Position.x;
		position.y = Position.y;
		position.z = Position.z;

		channels[channel]->setVolume(GetSound(sound)->GetVolume());
		result = system->playSound(GetSound(sound)->GetSound(), 0, true, &channels[channel]);
		result = channels[channel]->set3DAttributes(&position, &vel);
		result = channels[channel]->setPaused(false);
		
		if (!succeededOrWarn("Error", result))
			return Sound::DEFAULT;
		
		return channel;
	}

	int AudioManager::PlaySound(const std::string& sound, const int channel) {
		channels[channel]->setVolume(GetSound(sound)->GetVolume());
		result = system->playSound(GetSound(sound)->GetSound(), 0, true, &channels[channel]);
		result = channels[channel]->set3DAttributes(GetSound(sound)->GetPositionFmod(), &vel);
		result = channels[channel]->setPaused(false);
		if (!succeededOrWarn("Error", result))
			return Sound::DEFAULT;
		return channel;
	}

	void AudioManager::PauseSound(const std::string& sound) {

	}
	
	void AudioManager::StopSound(const std::string& sound) {

	}
}
