#include "Cube.h"

Cube::Cube(glm::vec3 position, glm::vec3 min, glm::vec3 max, std::string name) {
	this->min = min;
	this->max = max;
	this->position = position;
	this->width = max.x - min.x;
	this->depth = max.z - min.z;
	this->height = max.y - min.y;
	this->name = name;
}

Cube::Cube(glm::vec3 position, float width, float height, float depth, std::string name) {
	this->position = position;
	this->width = width;
	this->depth = depth;
	this->height = height;
	min = glm::vec3(position.x - width / 2, position.y - height / 2, position.z - depth / 2);
	max = glm::vec3(position.x + width / 2, position.y + height / 2, position.z + depth / 2);
	this->name = name;
}

Cube::Cube(GameObject* gameObject, std::string name) {
	std::vector<glm::vec3> vertices = gameObject->getIndexedVertices();

	glm::vec4 startVert = glm::vec4(vertices[0].x, vertices[0].y, vertices[0].z, 1) * (glm::rotate(glm::mat4(1), -gameObject->getRotation().y, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), gameObject->getRotation().x, glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), gameObject->getRotation().z, glm::vec3(0, 0, 1))) * glm::scale(glm::mat4(1), gameObject->getScale());

	float minx = startVert.x;
	float maxx = startVert.x;
	float miny = startVert.y;
	float maxy = startVert.y;
	float minz = startVert.z;
	float maxz = startVert.z;

	for (int i = 0; i < vertices.size()-1; i++) {
		glm::vec4 tempVec(vertices[i].x, vertices[i].y, vertices[i].z, 1);
		tempVec = tempVec * (glm::rotate(glm::mat4(1), gameObject->getRotation().y, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), gameObject->getRotation().x, glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), gameObject->getRotation().z, glm::vec3(0, 0, 1))) * glm::scale(glm::mat4(1), gameObject->getScale());
		if (tempVec.x < minx)
			minx = tempVec.x;
		if (tempVec.x > maxx)
			maxx = tempVec.x;

		if (tempVec.y < miny)
			miny = tempVec.y;
		if (tempVec.y > maxy)
			maxy = tempVec.y;

		if (tempVec.z < minz)
			minz = tempVec.z;
		if (tempVec.z > maxx)
			maxz = tempVec.z;
	}

	// 1.05 is for padding because the camera can sometimes clip into the object
	width = (maxx - minx) * 1.0f;
	height = (maxy - miny) * 1.0f;
	depth = (maxz - minz) * 1.0f;

	this->position = glm::vec3(gameObject->getPosition().x + (minx + maxx) / 2, gameObject->getPosition().y + (miny + maxy) / 2, gameObject->getPosition().z + (minz + maxz) / 2);
	min = glm::vec3(position.x - width / 2, position.y - height / 2, position.z - depth / 2);
	max = glm::vec3(position.x + width / 2, position.y + height / 2, position.z + depth / 2);

	this->name = name;
}

std::string Cube::GetName() {
	return this->name;
}

std::string Cube::GetTag() {
	return this->tag;
}

void Cube::SetTag(const std::string& tag) {
	this->tag = tag;
}

void Cube::setPosition(const glm::vec3 position) {
	this->position = position;
	min = glm::vec3(position.x - width / 2, position.y - height / 2, position.z - depth / 2);
	max = glm::vec3(position.x + width / 2, position.y + height / 2, position.z + depth / 2);
}

glm::vec3 Cube::getPosition() {
	return position;
}

void Cube::SetStatic(bool isStatic) {
	this->isStatic = isStatic;
}

bool Cube::GetStatic() const {
	if (this == nullptr)
		return false;
	return isStatic;
}

float Cube::getDepth() const {
	return depth;
}

float Cube::getHeight() const {
	return height;
}

float Cube::getWidth() const {
	return width;
}

void Cube::SetDelete(bool Delete) {
	shouldDelete = Delete;
}

bool Cube::ShouldDelete() const {
	return shouldDelete;
}

void Cube::setDimensions(float width, float height, float depth) {
	this->depth = depth;
	this->height = height;
	this->width = width;

	min = glm::vec3(position.x - width / 2, position.y - height / 2, position.z - depth / 2);
	max = glm::vec3(position.x + width / 2, position.y + height / 2, position.z + depth / 2);
}

bool Cube::GetIsTrigger() const {
	return isTrigger;
}

glm::vec3 Cube::getMin() const {
	return min;
}

glm::vec3 Cube::getMax() const {
	return max;
}

void Cube::SetIsTrigger(const bool trigger) {
	isTrigger = trigger;
}

bool Cube::TouchingRight(const Cube* collider, const float velocity) const {
	return this->getMax().x + velocity > collider->getMin().x &&
		this->getMin().x < collider->getMin().x &&
		this->getMax().z > collider->getMin().z &&
		this->getMin().z < collider->getMax().z &&
		this->getMin().y < collider->getMax().y &&
		this->getMax().y > collider->getMin().y;
}

bool Cube::TouchingLeft(const Cube* collider, const float velocity) const {
	return this->getMin().x + velocity < collider->getMax().x &&
		this->getMax().x > collider->getMax().x &&
		this->getMax().z > collider->getMin().z &&
		this->getMin().z < collider->getMax().z &&
		this->getMin().y < collider->getMax().y &&
		this->getMax().y > collider->getMin().y;
}

bool Cube::TouchingFront(const Cube* collider, const float velocity) const {
	return this->getMin().z + velocity < collider->getMax().z &&
		this->getMax().z > collider->getMax().z &&
		this->getMax().x > collider->getMin().x &&
		this->getMin().x < collider->getMax().x &&
		this->getMin().y < collider->getMax().y &&
		this->getMax().y > collider->getMin().y;
}

bool Cube::TouchingBack(const Cube* collider, const float velocity) const {
	return this->getMax().z + velocity > collider->getMin().z &&
		this->getMin().z < collider->getMin().z &&
		this->getMax().x > collider->getMin().x &&
		this->getMin().x < collider->getMax().x &&
		this->getMin().y < collider->getMax().y &&
		this->getMax().y > collider->getMin().y;
}

bool Cube::TouchingBottom(const Cube* collider, const float velocity) const {
	return this->getMin().y < collider->getMax().y &&
		this->getMax().x > collider->getMin().x &&
		this->getMin().x < collider->getMax().x &&
		this->getMax().z > collider->getMin().z &&
		this->getMin().z < collider->getMax().z;
}

bool Cube::TouchingTop(const Cube* collider, const float velocity) const {
	return this->getMax().y > collider->getMin().y &&
		this->getMax().x > collider->getMin().x &&
		this->getMin().x < collider->getMax().x &&
		this->getMax().z > collider->getMin().z &&
		this->getMin().z < collider->getMax().z;
}

// Returns -1 if there is no intersection
float Cube::intersect(const Ray& r, const float t0, const float t1) const {
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	if (r.direction.x >= 0) {
		tmin = (min.x - r.origin.x) / r.direction.x;
		tmax = (max.x - r.origin.x) / r.direction.x;
	}
	else {
		tmin = (max.x - r.origin.x) / r.direction.x;
		tmax = (min.x - r.origin.x) / r.direction.x;
	}
	if (r.direction.y >= 0) {
		tymin = (min.y - r.origin.y) / r.direction.y;
		tymax = (max.y - r.origin.y) / r.direction.y;
	}
	else {
		tymin = (max.y - r.origin.y) / r.direction.y;
		tymax = (min.y - r.origin.y) / r.direction.y;
	}

	if ((tmin > tymax) || (tymin > tmax))
		return -1;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	if (r.direction.z >= 0) {
		tzmin = (min.z - r.origin.z) / r.direction.z;
		tzmax = (max.z - r.origin.z) / r.direction.z;
	}
	else {
		tzmin = (max.z - r.origin.z) / r.direction.z;
		tzmax = (min.z - r.origin.z) / r.direction.z;
	}
	if ((tmin > tzmax) || (tzmin > tmax))
		return -1;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;
	if ((tmin < t1) && (tmax > t0))
		return tmin;
	return -1;
}

void Cube::Regenerate(GameObject* gameObject) {
	std::vector<glm::vec3> vertices = gameObject->getIndexedVertices();

	glm::vec4 startvert = glm::vec4(vertices[0].x, vertices[0].y, vertices[0].z, 1) * (glm::rotate(glm::mat4(1), -gameObject->getRotation().y, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), gameObject->getRotation().x, glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), gameObject->getRotation().z, glm::vec3(0, 0, 1))) * glm::scale(glm::mat4(1), gameObject->getScale());
	glm::vec4 lastvert = glm::vec4(vertices[1].x, vertices[1].y, vertices[1].z, 1) * (glm::rotate(glm::mat4(1), -gameObject->getRotation().y, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), gameObject->getRotation().x, glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), gameObject->getRotation().z, glm::vec3(0, 0, 1))) * glm::scale(glm::mat4(1), gameObject->getScale());

	float minx = startvert.x;
	float maxx = startvert.x;
	float miny = startvert.y;
	float maxy = lastvert.y;
	float minz = lastvert.z;
	float maxz = lastvert.z;

	for (int i = 0; i < vertices.size() - 1; i++) {
		glm::vec4 tempVec(vertices[i].x, vertices[i].y, vertices[i].z, 1);
		tempVec = tempVec * (glm::rotate(glm::mat4(1), -gameObject->getRotation().y, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), gameObject->getRotation().x, glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), gameObject->getRotation().z, glm::vec3(0, 0, 1))) * glm::scale(glm::mat4(1), gameObject->getScale());
		if (tempVec.x < minx)
			minx = tempVec.x;
		if (tempVec.x > maxx)
			maxx = tempVec.x;

		if (tempVec.y < miny)
			miny = tempVec.y;
		if (tempVec.y > maxy)
			maxy = tempVec.y;

		if (tempVec.z < minz)
			minz = tempVec.z;
		if (tempVec.z > maxx)
			maxz = tempVec.z;
	}

	//1.05 is for padding because the camera can somtimes clip into the object
	width = (maxx - minx) * 1.0f;
	height = (maxy - miny) * 1.0f;
	depth = (maxz - minz) * 1.0f;

	this->position = glm::vec3(gameObject->getPosition().x + (minx + maxx) / 2, gameObject->getPosition().y + (miny + maxy) / 2, gameObject->getPosition().z + (minz + maxz) / 2);
	min = glm::vec3(position.x - width / 2, position.y - height / 2, position.z - depth / 2);
	max = glm::vec3(position.x + width / 2, position.y + height / 2, position.z + depth / 2);
}