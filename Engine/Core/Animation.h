#pragma once

// TODO: This is still a work-in-progress and hasn't been implemented into the engine

struct KeyFrame;

class Animation
{
public:
	Animation(const std::string& name);
	Animation(const std::vector<KeyFrame>& keyframes, const std::string& name);

	void AddKeyFrame(const KeyFrame& keyframe);
	size_t GetKeyFrameSize() const;
	bool Playing() const;
	void Stop();
	void Start();
	void Pause();
	void SetKeyFrame(int keyframeIndex);
	void Transform();
	std::string GetName();
	void SetGameObject(GameObject* gameObject);

private:
	void NextKeyFrame();
	std::vector<KeyFrame> keyframes;

	float timeStart = 0;
	int currentKeyFrame = -1;
	bool playing = false;

	glm::vec3 startingPosition;
	glm::vec3 startingRotation;

	std::string name;

	GameObject* gameObject;
};

namespace AnimationManager
{
	void AddAnimation(const Animation& animation);
	void Play(const std::string& Name, const std::string& ObjectName);
	void Stop(const std::string& Name);
	void Pause(const std::string& Name);
	void Update(float deltaTime);
	Animation* GetAnimation(const std::string& Name);
}
