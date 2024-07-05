#pragma once
#include "GameObject.h"
#include "Engine/Core/AssetManager.h"



GameObject::GameObject(std::string name) {
	this->name = name;
	parentName = "";

}
GameObject::GameObject(std::string name, glm::vec3 position) {
	this->name = name;
	setPosition(position);
	parentName = "";

}
GameObject::GameObject(std::string name, const char* path) {
	this->name = name;
	LoadModel(path);
	parentName = "";

}
GameObject::GameObject(std::string name, const char* path, glm::vec3 position) {
	this->name = name;
	LoadModel(path);
	setPosition(position);
	parentName = "";

}
GameObject::GameObject(std::string name, const char* path, Texture* texture, glm::vec3 position) {
	this->name = name;
	setPosition(position);
	this->texture = texture;
	LoadModel(path);
	parentName = "";
}

GameObject::GameObject(std::string name, std::string parentname, Texture* texture, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::vector<unsigned short> indice,
	std::vector<glm::vec3> indexed_vert, std::vector<glm::vec2> indexed_uv, std::vector<glm::vec3> indexed_norms)
{
	this->name = name;
	setPosition(position);
	this->texture = texture;
	parentName = parentname;
	setRotation(rotation);
	transform.scale.x = scale.x;
	transform.scale.y = scale.y;
	transform.scale.z = scale.z;
	indices = indice;
	indexed_normals = indexed_norms;
	indexed_uvs = indexed_uv;
	this->indexed_vertices = indexed_vert;

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_DYNAMIC_DRAW);


	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_DYNAMIC_DRAW);


	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_DYNAMIC_DRAW);

	// Generate a buffer for the indices as well

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_DYNAMIC_DRAW);
}

void GameObject::LoadModel(const char* path) {
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	bool res = loader::loadOBJ(path, vertices, uvs, normals);
	indexer::indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_DYNAMIC_DRAW);


	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_DYNAMIC_DRAW);


	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_DYNAMIC_DRAW);

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_DYNAMIC_DRAW);
}

//Parent child transformations
glm::mat4 GameObject::GetModelMatrix() {
	glm::mat4 matrix = transform.to_mat4();
	if (parentName != "") {
		GameObject* parent = AssetManager::GetGameObject(parentName);
		if (parent != NULL) {
			matrix = transform.to_mat4() * parent->GetModelMatrix();
		}
	}
	return  matrix;
}
//gets the models world space without translations from the parent to the world and not the parent
glm::mat4 GameObject::GetLocalModelMatrix() {
	return transform.to_mat4();
}

void GameObject::RenderObject(GLuint& programID) {
	glUseProgram(programID);

	if (texture != NULL)
	{
		glActiveTexture(texture->GetTextureNumber() + GL_TEXTURE0);
		GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
		glBindTexture(GL_TEXTURE_2D, texture->GetTexture());
		glUniform1i(TextureID, texture->GetTextureNumber());
	}
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	// Draw the triangles !

	glDrawElements(
		GL_TRIANGLES,      // mode
		indices.size(),    // count
		GL_UNSIGNED_SHORT,   // type
		(void*)0           // element array buffer offset
	);


	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


void GameObject::setPosition(glm::vec3 position) {
	transform.position = position;
}
void GameObject::setRotation(glm::vec3 rotation) {
	transform.rotation = rotation;
}
void GameObject::setScale(glm::vec3 scale) {
	transform.scale = scale;
}

glm::vec3 GameObject::getPosition() {
	return transform.position;
}
glm::vec3 GameObject::getRotation() {
	return transform.rotation;
}
glm::vec3 GameObject::getScale() {
	return transform.scale;
}

void GameObject::setPositionX(float x) {
	transform.position.x = x;
}
void GameObject::setPositionY(float y) {
	transform.position.y = y;
}
void GameObject::setPositionZ(float z) {
	transform.position.z = z;
}

void GameObject::SetRotationX(float x) {
	transform.rotation.x = x;
}
void GameObject::SetRotationY(float y) {
	transform.rotation.y = y;
}
void GameObject::SetRotationZ(float z) {
	transform.rotation.z = z;

}

void GameObject::SetScale(float scale) {
	transform.scale = glm::vec3(scale);
}

std::string GameObject::GetName() {
	return name;
}
std::string GameObject::GetParentName() {
	return parentName;
}
void GameObject::SetParentName(std::string name) {
	parentName = name;
}


std::vector<unsigned short> GameObject::getIndices() {
	return indices;
}
std::vector<glm::vec3>  GameObject::getIndexedVerticies() {
	return indexed_vertices;
}
std::vector<glm::vec2>  GameObject::getIndexedUvs() {
	return indexed_uvs;
}
std::vector<glm::vec3>  GameObject::getIndexedNormals() {
	return indexed_normals;
}
const char* GameObject::GetTextureName() {
	return texture->GetName();
}