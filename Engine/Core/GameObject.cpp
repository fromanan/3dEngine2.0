#include "pch.h"

#ifdef max
#undef max
#endif

#include "GameObject.h"

#include "AssetManager.h"
#include "CollisionGroups.h"
#include "PhysicsManagerBullet.h"

#include "Loaders/Loader.hpp"
#include "Loaders/Indexer.h"

GameObject::GameObject() = default;

GameObject::GameObject(const std::string& name, bool save, float mass, ColliderShape shape) {
	this->name = name;
	this->parentName = "";
}

GameObject::GameObject(const std::string& name, const glm::vec3 position, const bool save, float mass, ColliderShape shape) {
	this->name = name;
	setPosition(position);
	this->parentName = "";
	this->canSave = save;
}

GameObject::GameObject(const std::string& name, Model* model, glm::vec3 position, bool save, float mass, ColliderShape shape) {
	this->name = name;
	this->model = model;
	this->parentName = "";
	this->canSave = save;

	float width = 1;
	float height = 1;
	float depth = 1;
	this->Btransform.setOrigin(glmToBtVector3(position));
	glm::vec3 dimensions(1, 1, 1);

	if (shape == Box) {
		glm::vec3 minPoint(std::numeric_limits<float>::max());
		glm::vec3 maxPoint(std::numeric_limits<float>::lowest());

		for (const auto& vertex : model->GetCurrentMesh()->indexed_vertices) {
			if (vertex.x < minPoint.x) minPoint.x = vertex.x;
			if (vertex.y < minPoint.y) minPoint.y = vertex.y;
			if (vertex.z < minPoint.z) minPoint.z = vertex.z;

			if (vertex.x > maxPoint.x) maxPoint.x = vertex.x;
			if (vertex.y > maxPoint.y) maxPoint.y = vertex.y;
			if (vertex.z > maxPoint.z) maxPoint.z = vertex.z;
		}
		dimensions = maxPoint - minPoint;
		this->collider = new btBoxShape(btVector3(dimensions.x / 2, dimensions.y / 2, dimensions.z / 2));

	}
	else if (shape == Convex) {
		this->convexHullShape = new btConvexHullShape();
		for (const auto& vertex : model->GetCurrentMesh()->indexed_vertices) {
			this->convexHullShape->addPoint(glmToBtVector3(vertex));
		}
		this->convexHullShape->optimizeConvexHull();
	}
	else if (shape == Sphere) {
		glm::vec3 minPoint(std::numeric_limits<float>::max());
		glm::vec3 maxPoint(std::numeric_limits<float>::lowest());

		for (const auto& vertex : model->GetCurrentMesh()->indexed_vertices) {
			if (vertex.x < minPoint.x) minPoint.x = vertex.x;
			if (vertex.y < minPoint.y) minPoint.y = vertex.y;
			if (vertex.z < minPoint.z) minPoint.z = vertex.z;

			if (vertex.x > maxPoint.x) maxPoint.x = vertex.x;
			if (vertex.y > maxPoint.y) maxPoint.y = vertex.y;
			if (vertex.z > maxPoint.z) maxPoint.z = vertex.z;
		}
		dimensions = maxPoint - minPoint;
		this->collider = new btSphereShape(dimensions.x / 2);
	}
	else if (shape == Capsule) {
		glm::vec3 minPoint(std::numeric_limits<float>::max());
		glm::vec3 maxPoint(std::numeric_limits<float>::lowest());

		for (const auto& vertex : model->GetCurrentMesh()->indexed_vertices) {
			if (vertex.x < minPoint.x) minPoint.x = vertex.x;
			if (vertex.y < minPoint.y) minPoint.y = vertex.y;
			if (vertex.z < minPoint.z) minPoint.z = vertex.z;

			if (vertex.x > maxPoint.x) maxPoint.x = vertex.x;
			if (vertex.y > maxPoint.y) maxPoint.y = vertex.y;
			if (vertex.z > maxPoint.z) maxPoint.z = vertex.z;
		}
		dimensions = maxPoint - minPoint;
		this->collider = new btCapsuleShape(dimensions.x / 2, dimensions.y / 2);
	}

	PhysicsManagerBullet::AddColliderShape(collider);
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic && collider != nullptr)
		this->collider->calculateLocalInertia(mass, localInertia);
	else if (isDynamic && convexHullShape != nullptr)
		this->convexHullShape->calculateLocalInertia(mass, localInertia);

	this->Btransform.setIdentity();
	this->Btransform.setOrigin(btVector3(position.x, position.y, position.z));

	// Using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(Btransform);
	if (convexHullShape == nullptr) {
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collider, localInertia);
		this->body = new btRigidBody(rbInfo);
	}
	else {
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, convexHullShape, localInertia);
		this->body = new btRigidBody(rbInfo);
	}

	this->body->setActivationState(DISABLE_DEACTIVATION);
	this->body->setFriction(0.7f);
	this->body->setUserIndex(-1);
	
	// Add the body to the dynamics world
	if (mass != 0)
		PhysicsManagerBullet::GetDynamicWorld()->addRigidBody(body, GROUP_DYNAMIC, GROUP_PLAYER | GROUP_STATIC | GROUP_DYNAMIC);
	else
		PhysicsManagerBullet::GetDynamicWorld()->addRigidBody(body, GROUP_STATIC, GROUP_PLAYER | GROUP_STATIC | GROUP_DYNAMIC);
	
	setPosition(position);
}


GameObject::GameObject(const std::string& name, Model* model, glm::vec3 position, bool save, float mass, ColliderShape shape, float width, float height, float depth) {
	this->name = name;
	this->model = model;
	this->parentName = "";
	this->canSave = save;

	this->Btransform.setOrigin(glmToBtVector3(position));

	if (shape == Sphere)
		this->collider = new btSphereShape(height / 2);
	else
		this->collider = new btBoxShape(btVector3(width / 2, height / 2, depth / 2));

	PhysicsManagerBullet::AddColliderShape(collider);
	const bool isDynamic = mass != 0.f;

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		this->collider->calculateLocalInertia(mass, localInertia);

	this->Btransform.setIdentity();
	this->Btransform.setOrigin(btVector3(position.x, position.y, position.z));

	// Using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(Btransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collider, localInertia);
	this->body = new btRigidBody(rbInfo);
	this->body->setActivationState(DISABLE_DEACTIVATION);
	this->body->setFriction(0.7f);
	this->body->setUserIndex(-1);
	
	// Add the body to the dynamics world
	if (mass != 0)
		PhysicsManagerBullet::GetDynamicWorld()->addRigidBody(body, GROUP_DYNAMIC, GROUP_PLAYER | GROUP_STATIC | GROUP_DYNAMIC);
	else
		PhysicsManagerBullet::GetDynamicWorld()->addRigidBody(body, GROUP_STATIC, GROUP_PLAYER | GROUP_STATIC | GROUP_DYNAMIC);
	
	setPosition(position);
}

void GameObject::Copy(std::string copyName) {
	//AssetManager::AddGameObject(GameObject(copyName, parentName, texture, getPosition(), getRotation(), getScale(), vertices, uvs, normals, tangents, bitangents, indices, indexed_vertices, indexed_uvs, indexed_normals, indexed_tangents, indexed_bitangents, canSave, render, shouldDelete,1,convexHullShape));
}

void GameObject::SetUserPoint(void* pointer) const {
	this->body->setUserPointer(pointer);
}

// Parent child transformations
glm::mat4 GameObject::GetModelMatrix() const {

	glm::mat4 matrix = this->transform.to_mat4();
	//Btransform.getOpenGLMatrix(glm::value_ptr(matrix));

	if (!parentName.empty()) {
		if (const GameObject* parent = AssetManager::GetGameObject(parentName); parent != nullptr) {
			matrix = parent->GetModelMatrix() * transform.to_mat4();
		}
	}
	
	return  matrix;
}

// Gets the models world space without translations from the parent to the world and not the parent
glm::mat4 GameObject::GetLocalModelMatrix() const {
	return this->transform.to_mat4();
}

btRigidBody* GameObject::GetRigidBody() const {
	return this->body;
}

btCollisionShape* GameObject::GetCollisionShape() const {
	return this->collider;
}

void GameObject::Update() {
	this->transform.position = glm::vec3(body->getWorldTransform().getOrigin().x(), body->getWorldTransform().getOrigin().y(), body->getWorldTransform().getOrigin().z());
	this->transform.rotation = glm::eulerAngles(glm::quat(body->getWorldTransform().getRotation().w(), body->getWorldTransform().getRotation().x(), body->getWorldTransform().getRotation().y(), body->getWorldTransform().getRotation().z()));
}

void GameObject::RenderObject(const GLuint& programID) const {
	if (!render)
		return;
	model->RenderModel(programID);
}

void GameObject::setPosition(const glm::vec3 position) {
	this->transform.position = position;
	btTransform& t = body->getWorldTransform();
	t.setOrigin(btVector3(position.x,position.y,position.z));
	this->body->getMotionState()->setWorldTransform(t);
}

void GameObject::setRotation(const glm::vec3 rotation) {
	this->transform.rotation = rotation;
	btTransform& t = body->getWorldTransform();
	btQuaternion quat;
	quat.setEuler(rotation.y, rotation.x, rotation.z);
	t.setRotation(quat);
	this->body->getMotionState()->setWorldTransform(t);
}

void GameObject::setScale(const glm::vec3 scale) {
	this->transform.scale = scale;
}

glm::vec3 GameObject::getPosition() const {
	return btToGlmVector3(body->getWorldTransform().getOrigin());
}

glm::vec3 GameObject::getRotation() const {
	return btQuatToGLMVec(body->getWorldTransform().getRotation());
}

glm::vec3 GameObject::getScale() const {
	return this->transform.scale;
}

void GameObject::addPosition(const glm::vec3 position) {
	this->transform.position += position;
	this->body->getWorldTransform().setOrigin(btVector3(transform.position.x, transform.position.y, transform.position.z));
	//this->body->getMotionState()->setWorldTransform(t);
}

void GameObject::setPositionX(const float x) {
	this->transform.position.x = x;
	this->body->getWorldTransform().setOrigin(btVector3(transform.position.x, transform.position.y, transform.position.z));
	//this->body->getMotionState()->setWorldTransform(t);
}

void GameObject::setPositionY(const float y) {
	this->transform.position.y = y;
	this->body->getWorldTransform().setOrigin(btVector3(transform.position.x, transform.position.y, transform.position.z));
	//this->body->getMotionState()->setWorldTransform(t);
}

void GameObject::setPositionZ(const float z) {
	this->transform.position.z = z;
	btTransform& t = body->getWorldTransform();
	t.setOrigin(btVector3(transform.position.x, transform.position.y, transform.position.z));
	this->body->getMotionState()->setWorldTransform(t);
}

void GameObject::SetRotationX(const float x) {
	this->transform.rotation.x = x;
	btTransform& t = body->getWorldTransform();
	btQuaternion quat;
	quat.setEuler(transform.rotation.y, transform.rotation.x, transform.rotation.z);
	t.setRotation(quat);
	this->body->getMotionState()->setWorldTransform(t);
}

void GameObject::SetRotationY(const float y) {
	this->transform.rotation.y = y;
	btTransform& t = body->getWorldTransform();
	btQuaternion quat;
	quat.setEuler(transform.rotation.y, transform.rotation.x, transform.rotation.z);
	t.setRotation(quat);
	this->body->getMotionState()->setWorldTransform(t);
}

void GameObject::SetRotationZ(const float z) {
	this->transform.rotation.z = z;
	btTransform& t = this->body->getWorldTransform();
	btQuaternion quat;
	quat.setEuler(transform.rotation.y, transform.rotation.x, transform.rotation.z);
	t.setRotation(quat);
	this->body->getMotionState()->setWorldTransform(t);
}

void GameObject::SetScale(const float scale) {
	this->transform.scale = glm::vec3(scale);
}

std::string GameObject::GetName() {
	return this->name;
}

std::string GameObject::GetParentName() {
	return this->parentName;
}

void GameObject::SetParentName(const std::string& name) {
	this->parentName = name;
}

std::vector<unsigned short> GameObject::getIndices() {
	return model->GetCurrentMesh()->indices;
}

std::vector<glm::vec3>  GameObject::getIndexedVerticies() {
	return model->GetCurrentMesh()->indexed_vertices;
}

std::vector<glm::vec2>  GameObject::getIndexedUvs() {
	return model->GetCurrentMesh()->indexed_uvs;
}

std::vector<glm::vec3>  GameObject::getIndexedNormals() {
	return model->GetCurrentMesh()->indexed_normals;
}

btConvexHullShape* GameObject::GetConvexHull() const {
	return this->convexHullShape;
}

const char* GameObject::GetTextureName() const {
	return this->model->GetTextureName();
}

bool GameObject::CanSave() const {
	return this->canSave;
}

void GameObject::SetRender(bool render) {
	this->render = render;
}

bool GameObject::ShouldRender() const {
	return this->render;
}

void GameObject::SetDelete(const bool state) {
	this->shouldDelete = state;
}

bool GameObject::ShouldDelete() const {
	return this->shouldDelete;
}

/*void GameObject::computeTangentBasis(
	// inputs
	std::vector<glm::vec3>& vertices,
	std::vector<glm::vec2>& uvs,
	std::vector<glm::vec3>& normals,
	// outputs
	std::vector<glm::vec3>& tangents,
	std::vector<glm::vec3>& bitangents
) {
	for (int i = 0; i < vertices.size(); i += 3) {
		// Shortcuts for vertices
		glm::vec3& v0 = vertices[i + 0];
		glm::vec3& v1 = vertices[i + 1];
		glm::vec3& v2 = vertices[i + 2];

		// Shortcuts for UVs
		glm::vec2& uv0 = uvs[i + 0];
		glm::vec2& uv1 = uvs[i + 1];
		glm::vec2& uv2 = uvs[i + 2];
		// Edges of the triangle : position delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		// UV delta
		const glm::vec2 deltaUV1 = uv1 - uv0;
		const glm::vec2 deltaUV2 = uv2 - uv0;

		const float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

		// Set the same tangent for all three vertices of the triangle.
		// They will be merged later, in vboindexer.cpp
		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		// Same thing for bitangents
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
	}
}*/
