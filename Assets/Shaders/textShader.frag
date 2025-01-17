#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Output data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D textShader;

void main(){

	color = texture( textShader, UV ) * vec4(0, 0, 0, 1);
	
	
} 