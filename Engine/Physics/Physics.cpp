#include "Physics.h"
#include "Camera.h"



Ray::Ray(glm::vec3 dir, glm::vec3 org) {
	direction = dir;
	origin = org;
}
Ray::Ray() {
	direction = glm::vec3(0, 0, 0);
	origin = glm::vec3(0, 0, 0);
}
void Ray::UpdateRay(glm::vec3 dir, glm::vec3 org) {
	direction = dir;
	origin = org;
}


bool Ray::intersectsTriangle(std::vector<glm::vec3> verticies, glm::mat4 ModelMatrix) {
	int i = -1;
	while (i < verticies.size() - 3) {
		glm::vec4 point1 = glm::vec4(verticies[++i].x, verticies[++i].y, verticies[++i].z, 1) * ModelMatrix; //a
		glm::vec4 point2 = glm::vec4(verticies[++i].x, verticies[++i].y, verticies[++i].z, 1) * ModelMatrix; //b
		glm::vec4 point3 = glm::vec4(verticies[++i].x, verticies[++i].y, verticies[++i].z, 1) * ModelMatrix; //c


		glm::vec3 AB = glm::vec3(point2.x - point1.x, point2.y - point1.y, point2.z - point1.z);
		glm::vec3 AC = glm::vec3(point3.x - point1.x, point3.y - point1.y, point3.z - point1.z);

		glm::vec3 normal = glm::cross(AB, AC);
		float n_dot_d = glm::dot(direction, normal);

		if (n_dot_d > 0.0001f) {
			float n_dot_ps = glm::dot(normal, glm::vec3(point1.x, point1.y, point1.z) - origin);

			float t = n_dot_ps / n_dot_d;


			glm::vec3 planePoint = origin + direction * t;

			glm::vec3 testPA = glm::vec3(planePoint.x - point1.x, planePoint.y - point1.y, planePoint.z - point1.z);
			//glm::vec3 testC = vec3(planePoint.x - point3.x, planePoint.y - point3.y, planePoint.z- point3.z);
			glm::vec3 Testnormal = glm::cross(AB, testPA);
			if (glm::dot(Testnormal, normal))
				return true;
			else
				return false;
		}
		else {
			return false;
		}
	}
}



Cube::Cube(glm::vec3 postion, glm::vec3 min, glm::vec3 max, std::string name) {
	this->min = min;
	this->max = max;
	this->position = postion;
	this->width = max.x - min.x;
	this->depth = max.z - min.z;
	this->height = max.y - min.y;
	this->name = name;

	
}

Cube::Cube(glm::vec3 postion, float width, float height, float depth, std::string name) {
	this->position = postion;
	this->width = width;
	this->depth = depth;
	this->height = height;
	min = glm::vec3(postion.x - width / 2, postion.y - height / 2, postion.z - depth / 2);
	max = glm::vec3(postion.x + width / 2, postion.y + height / 2, postion.z + depth / 2);
	this->name = name;
}
Cube::Cube(GameObject* gameobject, std::string name) {
	std::vector<glm::vec3> vertices = gameobject->getIndexedVerticies();

	float minx = vertices[0].x;
	float maxx = vertices[0].x;
	float miny = vertices[0].y;
	float maxy = vertices[0].y;
	float minz = vertices[0].z;
	float maxz = vertices[0].z;

	for (int i = 0; i < vertices.size()-1; i++)
	{
		glm::vec4 tempVec(vertices[i].x, vertices[i].y, vertices[i].z, 1);
		tempVec = tempVec * (glm::rotate(glm::mat4(1), gameobject->getRotation().y, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), gameobject->getRotation().x, glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), gameobject->getRotation().z, glm::vec3(0, 0, 1))) * glm::scale(glm::mat4(1), gameobject->getScale());
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
	width = (maxx - minx) * 1.0;
	height = (maxy - miny) * 1.0;
	depth = (maxz - minz) * 1.0;


	this->position = glm::vec3(gameobject->getPosition().x + (minx + maxx) / 2, gameobject->getPosition().y + (miny + maxy) / 2, gameobject->getPosition().z + (minz + maxz) / 2);
	min = glm::vec3(position.x - width / 2, position.y - height / 2, position.z - depth / 2);
	max = glm::vec3(position.x + width / 2, position.y + height / 2, position.z + depth / 2);

	this->name = name;
}

std::string Cube::GetName() {
	return name;
}
void Cube::setPosition(glm::vec3 position) {
	this->position = position;
}
glm::vec3 Cube::getPosition() {
	return position;
}
void Cube::SetStatic(bool Static) {
	this->Static = Static;
}
bool Cube::GetStatic() {
	return Static;
}
float Cube::getDepth() {
	return depth;
}
float Cube::getHeight() {
	return height;
}
float Cube::getWidth() {
	return width;
}
bool Cube::GetIsTrigger() {
	return isTrigger;
}
glm::vec3 Cube::getMin() {
	return min;
}
glm::vec3 Cube::getMax() {
	return max;
}
void Cube::SetIsTrigger(bool trigger) {
	isTrigger = trigger;
}
bool Cube::TouchingLeft(Cube* colider, float velocity) {
	return this->position.x - this->width / 2 < colider->getPosition().x + colider->getWidth() / 2 &&
		this->position.x + this->width / 2 > colider->getPosition().x + colider->getWidth() / 2 &&
		this->position.z + this->depth / 2 < colider->getPosition().z + colider->getDepth() / 2 &&
		this->position.z - this->depth / 2 > colider->getPosition().z - colider->getDepth() / 2 &&
		this->position.y - this->height / 2 < colider->getPosition().y + colider->getHeight() / 2 &&
		this->position.y + this->height / 2 > colider->getPosition().y - colider->getHeight() / 2;
	
}
bool Cube::TouchingRight(Cube* colider, float velocity) {
	return this->position.x + this->width / 2 > colider->getPosition().x - colider->getWidth() / 2 &&
		this->position.x - this->width / 2 < colider->getPosition().x - colider->getWidth() / 2 &&
		this->position.z + this->depth / 2 < colider->getPosition().z + colider->getDepth() / 2 &&
		this->position.z - this->depth / 2 > colider->getPosition().z - colider->getDepth() / 2 &&
		this->position.y - this->height / 2 < colider->getPosition().y + colider->getHeight() / 2 &&
		this->position.y + this->height / 2 > colider->getPosition().y - colider->getHeight() / 2;
}
bool Cube::TouchingFront(Cube* colider, float velocity) {
	return this->position.z - this->depth / 2 < colider->getPosition().z + colider->getDepth() / 2 &&
		this->position.z + this->depth / 2 > colider->getPosition().z + colider->getDepth() / 2 && 
		this->position.x + this->width / 2 < colider->getPosition().x + colider->getWidth() / 2 &&
		this->position.x - this->width / 2 > colider->getPosition().x - colider->getWidth() / 2 &&
		this->position.y - this->height / 2 < colider->getPosition().y + colider->getHeight() / 2 &&
		this->position.y + this->height / 2 > colider->getPosition().y - colider->getHeight() / 2;
}
bool Cube::TouchingBack(Cube* colider, float velocity) {
	return this->position.z + this->depth / 2 > colider->getPosition().z - colider->getDepth() / 2 &&
		this->position.z - this->depth / 2 < colider->getPosition().z - colider->getDepth() / 2 &&
		this->position.x + this->width / 2 < colider->getPosition().x + colider->getWidth() / 2 &&
		this->position.x - this->width / 2 > colider->getPosition().x - colider->getWidth() / 2 &&
		this->position.y - this->height / 2 < colider->getPosition().y + colider->getHeight() / 2 &&
		this->position.y + this->height / 2 > colider->getPosition().y - colider->getHeight() / 2;;
}
bool Cube::TouchingBottom(Cube* colider, float velocity) {	
	return this->position.y - this->height / 2 < colider->getPosition().y + colider->getHeight() / 2 &&
		this->getPosition().x > colider->getPosition().x - colider->getWidth() / 2 &&
		this->position.x < colider->getPosition().x + colider->getWidth() / 2 &&
		this->getPosition().z > colider->getPosition().z - colider->getDepth() / 2 && 
		this->position.z < colider->getPosition().z + colider->getDepth() / 2;
}
bool Cube::TouchingTop(Cube* colider, float velocity) {
	return this->position.y + this->height / 2 > colider->getPosition().y - colider->getHeight() / 2 &&
		this->getPosition().x > colider->getPosition().x - colider->getWidth() / 2 && 
		this->position.x < colider->getPosition().x + colider->getWidth() / 2 &&
		this->getPosition().z > colider->getPosition().z - colider->getDepth() / 2 && 
		this->position.z < colider->getPosition().z + colider->getDepth() / 2;
}
//Returns -1 if there is no intersection
float Cube::intersect(Ray r, float t0, float t1) {
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
	else
		return -1;
}
void Cube::Regenerate(GameObject* gameobject) {
	std::vector<glm::vec3> vertices = gameobject->getIndexedVerticies();

	float minx = vertices[0].x;
	float maxx = vertices[0].x;
	float miny = vertices[0].y;
	float maxy = vertices[0].y;
	float minz = vertices[0].z;
	float maxz = vertices[0].z;

	for (int i = 0; i < vertices.size() - 1; i++)
	{
		glm::vec4 tempVec(vertices[i].x, vertices[i].y, vertices[i].z, 1);
		tempVec = tempVec * (glm::rotate(glm::mat4(1), -gameobject->getRotation().y, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), gameobject->getRotation().x, glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), gameobject->getRotation().z, glm::vec3(0, 0, 1))) * glm::scale(glm::mat4(1), gameobject->getScale());
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
	width = (maxx - minx) * 1.0;
	height = (maxy - miny) * 1.0;
	depth = (maxz - minz) * 1.0;


	this->position = glm::vec3(gameobject->getPosition().x + (minx + maxx) / 2, gameobject->getPosition().y + (miny + maxy) / 2, gameobject->getPosition().z + (minz + maxz) / 2);
	min = glm::vec3(position.x - width / 2, position.y - height / 2, position.z - depth / 2);
	max = glm::vec3(position.x + width / 2, position.y + height / 2, position.z + depth / 2);
}



RigidBody::RigidBody() {

}
RigidBody::RigidBody(glm::vec3 position, const char* name) {
	this->position = position;
	this->name = name;
}
glm::vec3 RigidBody::GetPostion() {
	return position;
}
void RigidBody::SetPostion(glm::vec3 position) {
	this->position = position;
}
const char* RigidBody::GetName() {
	return name;
}
void RigidBody::NewPosition(float deltaTime) {
	position += velocity * deltaTime;
}
void RigidBody::NewPositionY(float deltaTime) {
	position.y += velocity.y * deltaTime;
}
void RigidBody::AddForce(glm::vec3 force) {
	velocity += force;
}
void RigidBody::AddForceX(float force) {
	velocity.x += force;
}
void RigidBody::AddForceY(float force) {
	velocity.y += force;
}
void RigidBody::AddForceZ(float force) {
	velocity.z += force;
}
void RigidBody::SetForce(glm::vec3 force) {
	velocity = force;
}
void RigidBody::SetForceX(float force) {
	velocity.x = force;
}
void RigidBody::SetForceY(float force) {
	velocity.y = force;
}
void RigidBody::SetForceZ(float force) {
	velocity.z = force;
}
void RigidBody::RemoveForceX() {
	velocity.x = 0;
}
void RigidBody::RemoveForceY() {
	velocity.y = 0;
}
void RigidBody::RemoveForceZ() {
	velocity.z = 0;
}
void RigidBody::SetColider(Cube* colider) {
	this->colider = colider;
}
Cube* RigidBody::GetColider() {
	return colider;
}
glm::vec3 RigidBody::GetForce() {
	return velocity;
}



namespace PhysicsManager {
	std::vector<Cube> coliders;
	std::vector<RigidBody> rigidbodies;

	//forces
	float friction = 7;
	float Gravity = -12;

	bool UpdatedCamera = false;

	void PhysicsManager::Update(float deltaTime) {
		UpdatedCamera = false;
		for (int i = 0; i < rigidbodies.size(); i++) {
			//add friction so your not sliding
			rigidbodies[i].SetForceX(rigidbodies[i].GetForce().x / (1 + (deltaTime * friction)));
			rigidbodies[i].SetForceZ(rigidbodies[i].GetForce().z / (1 + (deltaTime * friction)));
			rigidbodies[i].SetForceY(rigidbodies[i].GetForce().y + Gravity	* deltaTime);
			//Do colider Calculation
			if (rigidbodies[i].GetColider() != NULL) {
				for (int col = 0; col < coliders.size(); col++) {
					if (!UpdatedCamera)
						Camera::CheckIntersectingWithRay(&coliders[col]);
					if (rigidbodies[i].GetColider()->GetName() == coliders[col].GetName() || coliders[col].GetIsTrigger())
						continue;
					if (rigidbodies[i].GetForce().x < 0 && rigidbodies[i].GetColider()->TouchingLeft(&coliders[col], rigidbodies[i].GetForce().x * deltaTime))
						rigidbodies[i].RemoveForceX();
					if (rigidbodies[i].GetForce().x > 0 && rigidbodies[i].GetColider()->TouchingRight(&coliders[col], rigidbodies[i].GetForce().x * deltaTime))
						rigidbodies[i].RemoveForceX();
					if (rigidbodies[i].GetForce().z > 0 && rigidbodies[i].GetColider()->TouchingBack(&coliders[col], rigidbodies[i].GetForce().z * deltaTime))
						rigidbodies[i].RemoveForceZ();
					if (rigidbodies[i].GetForce().z < 0 && rigidbodies[i].GetColider()->TouchingFront(&coliders[col], rigidbodies[i].GetForce().z * deltaTime))
						rigidbodies[i].RemoveForceZ();
					if (rigidbodies[i].GetForce().y > 0 && rigidbodies[i].GetColider()->TouchingTop(&coliders[col], rigidbodies[i].GetForce().y * deltaTime))
						rigidbodies[i].RemoveForceY();
					if (rigidbodies[i].GetForce().y < 0 && rigidbodies[i].GetColider()->TouchingBottom(&coliders[col], rigidbodies[i].GetForce().y * deltaTime))
						rigidbodies[i].RemoveForceY();
						
				}
				if (!UpdatedCamera)
					UpdatedCamera = true;
			}
			//add veloctiy to position
			rigidbodies[i].NewPosition(deltaTime);
		}
	}
	RigidBody* PhysicsManager::AddRigidbody(glm::vec3 position, const char* name) {
		rigidbodies.push_back(RigidBody(position, name));
		return &rigidbodies[rigidbodies.size() - 1];
	}
	Cube* PhysicsManager::AddCube(glm::vec3 postion, glm::vec3 min, glm::vec3 max, std::string name) {
		coliders.push_back(Cube(postion, min, max, name));
		return &coliders[coliders.size() - 1];
	}
	Cube* PhysicsManager::AddCube(glm::vec3 postion, float width, float height, float depth, std::string name) {
		coliders.push_back(Cube(postion, width, height,depth, name));
		return &coliders[coliders.size() - 1];
	}
	Cube* AddCube(GameObject* gameobject, std::string name) {
		coliders.push_back(Cube(gameobject, name));
		return NULL;
	}

	Cube* PhysicsManager::GetColider(std::string name) {
		for (int i = 0; i < coliders.size(); i++) {
			if (coliders[i].GetName() == name)
				return &coliders[i];
		}
	}
	RigidBody* PhysicsManager::GetRigidbody(const char* name) {
		for (int i = 0; i < rigidbodies.size(); i++) {
			if (std::strcmp(rigidbodies[i].GetName(), name) == 0)
				return &rigidbodies[i];
		}
	}
	void PhysicsManager::RemoveCube(std::string name) {
		for (int i = 0; i < coliders.size(); i++) {
			if (coliders[i].GetName() == name)
				coliders.erase(coliders.begin() + i);
		}
	}

};