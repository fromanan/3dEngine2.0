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

const char* Cube::getName() {
	return name;
}

void Cube::setPosition(glm::vec3 position) {
	this->position = position;
}
glm::vec3 Cube::getPosition() {
	return position;
}

bool Cube::TouchingLeft(Cube* colider, float offset) {
	return position.x - offset - width / 2 < colider->position.x + colider->width / 2 &&
		position.x + width / 2 > colider->position.x + colider->width / 2 &&
		position.z + depth / 2 < colider->position.z + colider->depth / 2 &&
		position.z - depth / 2 > colider->position.z - colider->depth / 2 &&
		position.y - height / 2 < colider->position.y + colider->height / 2 &&
		position.y + height / 2 > colider->position.y - colider->height / 2;
}
bool Cube::TouchingRight(Cube* colider, float offset) {
	return position.x + offset + width / 2 > colider->position.x - colider->width / 2 &&
		position.x - width / 2 < colider->position.x - colider->width / 2 &&
		position.z + depth / 2 < colider->position.z + colider->depth / 2 &&
		position.z - depth / 2 > colider->position.z - colider->depth / 2 &&
		position.y - height / 2 < colider->position.y + colider->height / 2 &&
		position.y + height / 2 > colider->position.y - colider->height / 2;;
}
bool Cube::TouchingFront(Cube* colider, float offset) {
	return position.z - offset - depth / 2 < colider->position.z + colider->depth / 2 &&
		position.z + depth / 2 > colider->position.z + colider->depth / 2 &&
		position.x + width / 2 < colider->position.x + colider->width / 2 &&
		position.x - width / 2 > colider->position.x - colider->width / 2 &&
		position.y - height / 2 < colider->position.y + colider->height / 2 &&
		position.y + height / 2 > colider->position.y - colider->height / 2;;
}
bool Cube::TouchingBack(Cube* colider, float offset) {
	return position.z + offset + depth / 2 > colider->position.z - colider->depth / 2 &&
		position.z - depth / 2 < colider->position.z - colider->depth / 2 &&
		position.x + width / 2 < colider->position.x + colider->width / 2 &&
		position.x - width / 2 > colider->position.x - colider->width / 2 &&
		position.y - height / 2 < colider->position.y + colider->height / 2 &&
		position.y + height / 2 > colider->position.y - colider->height / 2;;
}

bool Cube::TouchingBottom(Cube* colider, float offset) {
	return position.y + offset - height / 2 < colider->position.y + colider->height / 2 &&
		position.x > colider->position.x - colider->width / 2 && position.x < colider->position.x + colider->width / 2 &&
		position.z > colider->position.z - colider->depth / 2 && position.z < colider->position.z + colider->depth / 2;
}

bool Cube::TouchingTop(Cube* colider, float offset) {
	return position.y + offset + height / 2 > colider->position.y - colider->height / 2 &&
		position.x > colider->position.x - colider->width / 2 && position.x < colider->position.x + colider->width / 2 &&
		position.z > colider->position.z - colider->depth / 2 && position.z < colider->position.z + colider->depth / 2;
}
