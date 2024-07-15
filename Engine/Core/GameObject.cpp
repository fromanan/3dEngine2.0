#pragma once
#include "GameObject.h"
#include "Engine/Core/AssetManager.h"



GameObject::GameObject(std::string name, bool save) {
	this->name = name;
	parentName = "";

}
GameObject::GameObject(std::string name, glm::vec3 position, bool save) {
	this->name = name;
	setPosition(position);
	parentName = "";
	canSave = save;


}
GameObject::GameObject(std::string name, const char* path, bool save) {
	this->name = name;
	LoadModel(path);
	parentName = "";
	canSave = save;


}
GameObject::GameObject(std::string name, const char* path, glm::vec3 position, bool save) {
	this->name = name;
	LoadModel(path);
	setPosition(position);
	parentName = "";
	canSave = save;


}
GameObject::GameObject(std::string name, const char* path, Texture* texture, glm::vec3 position, bool save) {
	this->name = name;
	setPosition(position);
	this->texture = texture;
	LoadModel(path);
	parentName = "";
	canSave = save;

}

GameObject::GameObject(std::string name, std::string parentname, Texture* texture, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::vector<unsigned short> indice,
	std::vector<glm::vec3> indexed_vert, std::vector<glm::vec2> indexed_uv, std::vector<glm::vec3> indexed_norms, bool save)
{
	canSave = save;
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
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);


	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);


	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
}

void GameObject::LoadModel(const char* path) {
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	bool res = loader::loadOBJ(path, vertices, uvs, normals);
	computeTangentBasis(vertices, uvs, normals, tangents, bitangents);
	indexer::indexVBO(vertices, uvs, normals, tangents, bitangents, indices, indexed_vertices, indexed_uvs, indexed_normals,indexed_tangents,indexed_bitangents);


	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);


	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);


	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &tangentbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_tangents.size() * sizeof(glm::vec3), &indexed_tangents[0], GL_STATIC_DRAW);

	glGenBuffers(1, &bitangentbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_bitangents.size() * sizeof(glm::vec3), &indexed_bitangents[0], GL_STATIC_DRAW);
}

//Parent child transformations
glm::mat4 GameObject::GetModelMatrix() {

	glm::mat4 matrix = transform.to_mat4();
	if (parentName != "") {
		GameObject* parent = AssetManager::GetGameObject(parentName);
		if (parent != NULL) {
			matrix = parent->GetModelMatrix() * transform.to_mat4();
		}
	}
	return  matrix;
}
//gets the models world space without translations from the parent to the world and not the parent
glm::mat4 GameObject::GetLocalModelMatrix() {
	return transform.to_mat4();
}

void GameObject::RenderObject(GLuint& programID) {
	if (!render)
		return;
	glUseProgram(programID);

	if (texture != NULL)
	{
		glActiveTexture(texture->GetTextureNumber() + GL_TEXTURE0);
		GLuint TextureID = glGetUniformLocation(programID, "DiffuseTextureSampler");
		glBindTexture(GL_TEXTURE_2D, texture->GetTexture());
		glUniform1i(TextureID, texture->GetTextureNumber());

		glActiveTexture(texture->GetTextureNormalNumber() + GL_TEXTURE0);
		GLuint NormalID = glGetUniformLocation(programID, "NormalTextureSampler");
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureNormal());
		glUniform1i(NormalID, texture->GetTextureNormalNumber());
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
	// 4th attribute buffer : tangents
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
	glVertexAttribPointer(
		3,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 5th attribute buffer : bitangents
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
	glVertexAttribPointer(
		4,                                // attribute
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
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
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
void GameObject::addPosition(glm::vec3 position) {
	transform.position += position;
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
bool GameObject::CanSave() {
	return canSave;
}
void GameObject::SetRender(bool render) {
	this->render = render;
}
void GameObject::SetDelete(bool state) {
	shouldDelete = state;
}
bool GameObject::ShouldDlete() {
	return shouldDelete;
}

void GameObject::computeTangentBasis(
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
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
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
}
