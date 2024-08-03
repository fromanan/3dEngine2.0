#include "pch.h"

#include "Loader.hpp"

#include "File.h"

namespace Loader
{
	GLuint Loader::loadBMP_custom(const char* imagepath) {
		// Data read from the header of the BMP file
		unsigned char header[54]; // Each BMP file begins by a 54-bytes header
		// Position in the file where the actual data begins
		// = width*height*3
		// Actual RGB data

		// Open the file
		FILE* file = fopen(imagepath, "rb");
		if (!file) {
			std::cout << "Image could not be opened at " << imagepath << std::endl;
			return 0;
		}

		if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
			std::cout << "Invalid BMP file provided" << std::endl;
			return false;
		}

		if (header[0] != 'B' || header[1] != 'M') {
			std::cout << "Invalid BMP file provided" << std::endl;
			return 0;
		}

		// Read ints from the byte array
		unsigned int dataPos = *reinterpret_cast<int*>(&header[0x0A]);
		unsigned int imageSize = *reinterpret_cast<int*>(&header[0x22]);
		const unsigned int width = *reinterpret_cast<int*>(&header[0x12]);
		const unsigned int height = *reinterpret_cast<int*>(&header[0x16]);

		// Some BMP files are misformatted, guess missing information
		if (imageSize == 0)
			imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
		if (dataPos == 0)
			dataPos = 54; // The BMP header is done that way

		// Create a buffer
		unsigned char* data = new unsigned char[imageSize];

		// Read the actual data from the file into the buffer
		fread(data, 1, imageSize, file);

		// Everything is in memory now, the file can be closed
		fclose(file);

		// Create one OpenGL texture
		GLuint textureID;
		glGenTextures(1, &textureID);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Give the image to OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, GL_BGR, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// When Minifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		// Generate mipmaps, by the way.
		glGenerateMipmap(GL_TEXTURE_2D);

		return textureID;
	}

	GLuint Loader::loadDDS(const char* imagepath) {
		unsigned char header[124];

		// Try to open the file 
		FILE* fp = fopen(imagepath, "rb");
		if (fp == nullptr)
			return 0;

		// Verify the type of file 
		char filecode[4];
		fread(filecode, 1, 4, fp);
		if (strncmp(filecode, "DDS ", 4) != 0) {
			fclose(fp);
			return 0;
		}

		// Get the surface desc 
		fread(&header, 124, 1, fp);

		unsigned int height = *reinterpret_cast<unsigned int*>(&header[8]);
		unsigned int width = *reinterpret_cast<unsigned int*>(&header[12]);
		const unsigned int linearSize = *reinterpret_cast<unsigned int*>(&header[16]);
		const unsigned int mipMapCount = *reinterpret_cast<unsigned int*>(&header[24]);
		const unsigned int fourCC = *reinterpret_cast<unsigned int*>(&header[80]);

		// How big is it going to be including all mipmaps? 
		unsigned int bufferSize = mipMapCount > 1 ? linearSize * 2 : linearSize;
		unsigned char* buffer = new unsigned char[bufferSize];
		fread(buffer, 1, bufferSize, fp);
		
		// Close the file pointer 
		fclose(fp);

		unsigned int components = fourCC == FOURCC_DXT1 ? 3 : 4;
		unsigned int format;
		switch (fourCC)
		{
		case FOURCC_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case FOURCC_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case FOURCC_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			delete[] buffer;
			return 0;
		}

		// Create one OpenGL texture
		GLuint textureID;
		glGenTextures(1, &textureID);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, textureID);

		const unsigned int blockSize = format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ? 8 : 16;
		unsigned int offset = 0;

		// Load the mipmaps
		for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
			unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
			glCompressedTexImage2D(GL_TEXTURE_2D, static_cast<GLint>(level), format, static_cast<GLsizei>(width), static_cast<GLsizei>(height),
				0, static_cast<GLsizei>(size), buffer + offset);

			offset += size;
			width /= 2;
			height /= 2;
		}
		
		delete[] buffer;

		return textureID;
	}

	bool Loader::loadOBJ(const char* path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals) {
		std::cout << "Loading OBJ file '" << path << "'" << std::endl;

		std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
		std::vector<glm::vec3> temp_vertices;
		std::vector<glm::vec2> temp_uvs;
		std::vector<glm::vec3> temp_normals;

		/*// Open the file using ifstream
		std::ifstream file(path);

		// confirm file opening
		if (!file.is_open()) {
			// print error message and return
			std::cout << "Failed to open file: '" << path << "'" << std::endl;
			return false;
		}

		// Read the file line by line into a string
		std::string line;
		while (getline(file, line)) {
			std::cout << line << std::endl;
			char lineHeader[128];
			
		}

		// Close the file
		file.close();*/
		
		FILE* file = fopen(path, "r");
		if (file == nullptr) {
			std::cout << "Could not open file at path '" << path << "'" << std::endl;
			return false;
		}

		while (true) {
			char lineHeader[128];
			// Read the first word of the line
			if (int res = fscanf(file, "%s", lineHeader); res == EOF)
				break; // EOF = End Of File. Quit the loop.

			// Else : parse lineHeader

			if (strcmp(lineHeader, "v") == 0) {
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0) {
				glm::vec2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
				temp_uvs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0) {
				glm::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					std::cout << "File could not be parsed! Try exporting with other options" << std::endl;
					return false;
				}

				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
			else {
				// Probably a comment, eat up the rest of the line
				char stupidBuffer[1000];
				fgets(stupidBuffer, 1000, file);
			}
		}

		// For each vertex of each triangle
		for (unsigned int i = 0; i < vertexIndices.size(); i++) {

			// Get the indices of its attributes
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int uvIndex = uvIndices[i];
			unsigned int normalIndex = normalIndices[i];

			// Get the attributes thanks to the index
			glm::vec3 vertex = temp_vertices[vertexIndex - 1];
			glm::vec2 uv = temp_uvs[uvIndex - 1];
			glm::vec3 normal = temp_normals[normalIndex - 1];

			// Put the attributes in buffers
			out_vertices.push_back(vertex);
			out_uvs.push_back(uv);
			out_normals.push_back(normal);
		}
		
		return true;
	}
}
