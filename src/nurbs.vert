#version 150

uniform mat4 projection;
uniform mat4 modelView;
uniform vec4 lightPosition;

in vec4 position;
in vec3 normal;
in vec2 uv;

void main(void)
{
	gl_Position = projection * modelView * position;
}