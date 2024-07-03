#include "Physics.h"



Ray::Ray(glm::vec3 dir, glm::vec3 org) {
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



Cube::Cube(glm::vec3 postion, glm::vec3 min, glm::vec3 max, const char* name) {
	this->min = min;
	this->max = max;
	this->position = postion;
	this->width = max.x - min.x;
	this->depth = max.z - min.z;
	this->height = max.y - min.y;
	this->name = name;

	
}

Cube::Cube(glm::vec3 postion, float width, float height, float depth, const char* name) {
	this->position = postion;
	this->width = width;
	this->depth = depth;
	this->height = height;
	min = glm::vec3(postion.x - width / 2, position.y - height / 2, postion.z - depth / 2);
	max = glm::vec3(postion.x + width / 2, position.y + height / 2, postion.z + depth / 2);
	this->name = name;
}
Cube::Cube(GameObject gameobject, const char* name) {
	std::vector<glm::vec3> vertices = gameobject.getIndexedVerticies();

	float minx = vertices[0].x;
	float maxx = vertices[0].x;
	float miny = vertices[0].y;
	float maxy = vertices[0].y;
	float minz = vertices[0].z;
	float maxz = vertices[0].z;
	for (int i = 0; i < vertices.size(); i++)
	{
		glm::vec4 tempVec(vertices[i].x, vertices[i].y, vertices[i].z, 1);
		tempVec = tempVec * (glm::rotate(glm::mat4(1), gameobject.getRotation().y, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), gameobject.getRotation().x, glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), gameobject.getRotation().z, glm::vec3(0, 0, 1))) * glm::scale(glm::mat4(1), gameobject.getScale());

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
	width = (maxx - minx) * 1.05;
	height = (maxy - miny) * 1.05;
	depth = (maxz - minz) * 1.05;

	min = glm::vec3(minx, miny, minz);
	max = glm::vec3(maxx, maxy, maxz);
	this->name = name;
}

const char* Cube::GetName() {
	return name;
}
void Cube::setPosition(glm::vec3 position) {
	this->position = position;
}
glm::vec3 Cube::getPosition() {
	return position;
}
bool Cube::TouchingLeft(Cube* colider, float velocity) {
	return position.x - velocity - width / 2 < colider->position.x + colider->width / 2 &&
		position.x + width / 2 > colider->position.x + colider->width / 2 &&
		position.z + depth / 2 < colider->position.z + colider->depth / 2 &&
		position.z - depth / 2 > colider->position.z - colider->depth / 2 &&
		position.y - height / 2 < colider->position.y + colider->height / 2 &&
		position.y + height / 2 > colider->position.y - colider->height / 2;
}
bool Cube::TouchingRight(Cube* colider, float velocity) {
	return position.x + velocity+ width / 2 > colider->position.x - colider->width / 2 &&
		position.x - width / 2 < colider->position.x - colider->width / 2 &&
		position.z + depth / 2 < colider->position.z + colider->depth / 2 &&
		position.z - depth / 2 > colider->position.z - colider->depth / 2 &&
		position.y - height / 2 < colider->position.y + colider->height / 2 &&
		position.y + height / 2 > colider->position.y - colider->height / 2;;
}
bool Cube::TouchingFront(Cube* colider, float velocity) {
	return position.z - velocity - depth / 2 < colider->position.z + colider->depth / 2 &&
		position.z + depth / 2 > colider->position.z + colider->depth / 2 &&
		position.x + width / 2 < colider->position.x + colider->width / 2 &&
		position.x - width / 2 > colider->position.x - colider->width / 2 &&
		position.y - height / 2 < colider->position.y + colider->height / 2 &&
		position.y + height / 2 > colider->position.y - colider->height / 2;;
}
bool Cube::TouchingBack(Cube* colider, float velocity) {
	return position.z + velocity + depth / 2 > colider->position.z - colider->depth / 2 &&
		position.z - depth / 2 < colider->position.z - colider->depth / 2 &&
		position.x + width / 2 < colider->position.x + colider->width / 2 &&
		position.x - width / 2 > colider->position.x - colider->width / 2 &&
		position.y - height / 2 < colider->position.y + colider->height / 2 &&
		position.y + height / 2 > colider->position.y - colider->height / 2;;
}
bool Cube::TouchingBottom(Cube* colider, float velocity) {
	return position.y + velocity - height / 2 < colider->position.y + colider->height / 2 &&
		position.x > colider->position.x - colider->width / 2 && position.x < colider->position.x + colider->width / 2 &&
		position.z > colider->position.z - colider->depth / 2 && position.z < colider->position.z + colider->depth / 2;
}
bool Cube::TouchingTop(Cube* colider, float velocity) {
	return position.y + velocity + height / 2 > colider->position.y - colider->height / 2 &&
		position.x > colider->position.x - colider->width / 2 && position.x < colider->position.x + colider->width / 2 &&
		position.z > colider->position.z - colider->depth / 2 && position.z < colider->position.z + colider->depth / 2;
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

	float friction = 0.992;

	void PhysicsManager::Update(float deltaTime) {
		for (int i = 0; i < rigidbodies.size(); i++) {
			//Do colider Calculation
			if (rigidbodies[i].GetColider() != NULL) {
				for (int col = 0; col < coliders.size(); col++) {
					if (rigidbodies[i].GetForce().x < 0 && rigidbodies[i].GetColider()->TouchingLeft(&coliders[col], rigidbodies[i].GetForce().x))
						rigidbodies[i].RemoveForceX();
					if (rigidbodies[i].GetForce().x > 0 && rigidbodies[i].GetColider()->TouchingRight(&coliders[col], rigidbodies[i].GetForce().x))
						rigidbodies[i].RemoveForceX();
					if (rigidbodies[i].GetForce().z > 0 && rigidbodies[i].GetColider()->TouchingBack(&coliders[col], rigidbodies[i].GetForce().z))
						rigidbodies[i].RemoveForceZ();
					if (rigidbodies[i].GetForce().z < 0 && rigidbodies[i].GetColider()->TouchingFront(&coliders[col], rigidbodies[i].GetForce().z))
						rigidbodies[i].RemoveForceZ();
					if (rigidbodies[i].GetForce().y > 0 && rigidbodies[i].GetColider()->TouchingTop(&coliders[col], rigidbodies[i].GetForce().y))
					{
						//need to implement
					}
					if (rigidbodies[i].GetForce().y < 0 && rigidbodies[i].GetColider()->TouchingBottom(&coliders[col], rigidbodies[i].GetForce().y))
						rigidbodies[i].RemoveForceY();
				}
			}
			//add friction so your not sliding
			rigidbodies[i].SetForceX(rigidbodies[i].GetForce().x * friction);
			rigidbodies[i].SetForceZ(rigidbodies[i].GetForce().z * friction);


			//add veloctiy to position
			rigidbodies[i].NewPosition(deltaTime);

		}
	}
	RigidBody* PhysicsManager::AddRigidbody(glm::vec3 position, const char* name) {
		rigidbodies.push_back(RigidBody(position, name));
		return &rigidbodies[rigidbodies.size() - 1];
	}
	Cube* PhysicsManager::AddCube(glm::vec3 postion, glm::vec3 min, glm::vec3 max, const char* name) {
		coliders.push_back(Cube(postion, min, max, name));
		return &coliders[coliders.size() - 1];
	}
	Cube* PhysicsManager::AddCube(glm::vec3 postion, float width, float height, float depth, const char* name) {
		coliders.push_back(Cube(postion, width, height,depth, name));
		return &coliders[coliders.size() - 1];
	}
	Cube* PhysicsManager::GetColider(const char* name) {
		for (int i = 0; i < coliders.size(); i++) {
			if (std::strcmp(coliders[i].GetName(), name) == 0)
				return &coliders[i];
		}
	}
	RigidBody* PhysicsManager::GetRigidbody(const char* name) {
		for (int i = 0; i < rigidbodies.size(); i++) {
			if (std::strcmp(rigidbodies[i].GetName(), name) == 0)
				return &rigidbodies[i];
		}
	}
};