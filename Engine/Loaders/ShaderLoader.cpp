#include "pch.h"

#include "ShaderLoader.hpp"

#include "File.h"

namespace ShaderLoader
{
	GLuint ShaderLoader::LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {
		GLuint VertexShaderID = CompileShader(vertex_file_path, GL_VERTEX_SHADER);
		if (VertexShaderID == 0)
			return 0;

		GLuint FragmentShaderID = CompileShader(fragment_file_path, GL_FRAGMENT_SHADER);
		if (FragmentShaderID == 0)
			return 0;

		GLint result = GL_FALSE;
		int info_log_length;

		// Link the program
		std::cout << "Linking program" << std::endl;
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &info_log_length);
		if (!result && info_log_length > 0) {
			std::vector<char> ProgramErrorMessage(info_log_length + 1);
			glGetProgramInfoLog(ProgramID, info_log_length, nullptr, ProgramErrorMessage.data());
			std::cout << ProgramErrorMessage.data() << std::endl;
		}

		glDetachShader(ProgramID, VertexShaderID);
		glDetachShader(ProgramID, FragmentShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		return ProgramID;
	}

	GLuint ShaderLoader::CompileShader(const char* shader_file_path, GLuint shaderType) {
		// Create the shaders
		GLuint shader_id = glCreateShader(shaderType);

		// Read the Vertex Shader code from the file
		std::string shader_code = File::read_file(shader_file_path);

		GLint result = GL_FALSE;
		int info_log_length;

		// Compile Shader
		std::cout << "Compiling shader : '" << shader_file_path << "'" << std::endl;
		char const* source_pointer = shader_code.c_str();
		glShaderSource(shader_id, 1, &source_pointer, nullptr);
		glCompileShader(shader_id);

		// Check Shader
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
		if (!result && info_log_length > 0) {
			std::vector<char> shader_error_message(info_log_length + 1);
			glGetShaderInfoLog(shader_id, info_log_length, nullptr, shader_error_message.data());
			std::cout << shader_error_message.data() << std::endl;
			return 0;
		}

		return shader_id;
	}
}
