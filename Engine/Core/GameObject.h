#pragma once

#include "ColliderShape.h"
#include "Texture.h"
#include "Transform.h"

class GameObject
{
public:
	GameObject();
	GameObject(const std::string& name, bool save, float mass, ColliderShape shape);
	GameObject(const std::string& name, glm::vec3 position, bool save, float mass, ColliderShape shape);
	GameObject(const std::string& name, Model* model, glm::vec3 position, bool save, float mass, ColliderShape shape);
	GameObject(const std::string& name, Model* model, glm::vec3 position, bool save, float mass, ColliderShape shape, float width, float height, float depth);

	void LoadModel(const char* path);
	void Copy(const std::string& copyName) const;
	glm::mat4 GetModelMatrix() const;
	glm::mat4 GetLocalModelMatrix() const;
	
	void RenderObject(const GLuint& programID) const;

	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);

	void Update();

	glm::vec3 getPosition() const;
	glm::vec3 getRotation() const;
	glm::vec3 getScale() const;

	void addPosition(glm::vec3 position);

	void setPositionX(float x);
	void setPositionY(float y);
	void setPositionZ(float z);

	void SetRotationX(float rotation);
	void SetRotationY(float rotation);
	void SetRotationZ(float rotation);

	void SetScale(float scale);

	std::string GetName();
	std::string GetParentName();

	void SetParentName(const std::string& name);

	// For Json Saving
	std::vector<unsigned short> getIndices();
	std::vector<glm::vec3> getIndexedVertices();
	std::vector<glm::vec2> getIndexedUvs();
	std::vector<glm::vec3> getIndexedNormals();
	const char* GetTextureName() const;

	bool CanSave() const;
	void SetRender(bool render);
	bool ShouldRender() const;

	void SetDelete(bool state);
	bool ShouldDelete() const;

	void SetUserPoint(void* pointer) const;

	/*static void computeTangentBasis(
		// inputs
		std::vector<glm::vec3>& vertices,
		std::vector<glm::vec2>& uvs,
		std::vector<glm::vec3>& normals,
		// outputs
		std::vector<glm::vec3>& tangents,
		std::vector<glm::vec3>& bitangents
	);*/

	btRigidBody* GetRigidBody() const;
	btCollisionShape* GetCollisionShape() const;
	btConvexHullShape* GetConvexHull() const;

private:
	Transform transform = Transform();
	Model* model = nullptr;

	std::string parentName;
	std::string name;
	
	// Physics stuff
	btCollisionShape* collider = nullptr;
	btConvexHullShape* convexHullShape = nullptr;
	btTransform Btransform;
	btRigidBody* body;
	
	bool canSave = false;
	bool render = true;
	bool shouldDelete = false;
};
