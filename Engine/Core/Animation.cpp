#include "Animation.h"

void KeyFrame::Create(const glm::vec3 position, const glm::vec3 rotation, const float duration) {
	this->position = position;
	this->rotation = rotation;
	this->duration = duration;
}

Animation::Animation(const std::string& name) {
	this->name = name;
}

Animation::Animation(const std::vector<KeyFrame>& keyframes, const std::string& name) {
	this->keyframes = keyframes;
	this->name = name;
}

void Animation::AddKeyFrame(const KeyFrame& keyframe) {
	this->keyframes.push_back(keyframe);
}

size_t Animation::GetKeyFrameSize() const {
	return this->keyframes.size();
}

bool Animation::Playing() const {
	return this->playing;
}

void Animation::Stop() {
	this->playing = false;
	this->currentKeyFrame = -1;
	this->gameObject = nullptr;
}

void Animation::Start() {
	this->playing = true;
}

void Animation::Pause() {
	this->playing = false;
}

void Animation::SetKeyFrame(const int keyframeIndex) {
	if (keyframeIndex < keyframes.size())
		this->currentKeyFrame = keyframeIndex;
}

void Animation::SetGameObject(GameObject* gameObject) {
	this->gameObject = gameObject;
}

void Animation::Transform() {
	const float t = glfwGetTime() / timeStart + keyframes[currentKeyFrame].duration;
	if (t >= 1) {
		NextKeyFrame();
		return;
	}
	const glm::vec3 newPosition = (1 - t) * startingPosition  + t * keyframes[currentKeyFrame].position;
	const glm::vec3 newRotation = (1 - t) * startingRotation + t * keyframes[currentKeyFrame].rotation;

	gameObject->setPosition(newPosition);
	gameObject->setRotation(newRotation);
}

void Animation::NextKeyFrame() {
	if (currentKeyFrame < keyframes.size()) {
		startingPosition = gameObject->getPosition();
		startingRotation = gameObject->getRotation();
		currentKeyFrame++;
		timeStart = glfwGetTime();
	}
	else {
		Stop();
	}
}

std::string Animation::GetName() {
	return this->name;
}

namespace AnimationManager
{
	std::vector <Animation> animations;
	
	void AnimationManager::AddAnimation(const Animation& animation) {
		animations.emplace_back(animation);
	}
	
	void AnimationManager::Play(const std::string& Name, const std::string& ObjectName) {
		GetAnimation(Name)->SetGameObject(AssetManager::GetGameObject(ObjectName));
		GetAnimation(Name)->Start();
	}
	
	void AnimationManager::Stop(const std::string& Name) {
		GetAnimation(Name)->Stop();
	}
	
	void AnimationManager::Pause(const std::string& Name) {
		GetAnimation(Name)->Pause();
	}

	void AnimationManager::Update(float deltaTime) {
		for (auto& animation : animations) {
			if (animation.Playing()) {
				animation.Transform();
			}
		}
	}
	
	Animation* AnimationManager::GetAnimation(const std::string& Name) {
		for (auto& animation : animations) {
			if (animation.GetName() == Name)
				return &animation;
		}
		return nullptr;
	}
}
