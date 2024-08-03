#pragma once

namespace ShaderLoader
{
	GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
	GLuint CompileShader(const char* shader_file_path, GLuint shaderType);
}
