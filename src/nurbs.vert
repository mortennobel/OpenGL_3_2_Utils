#version 150

uniform mat4 projection;
uniform mat4 modelView;


in vec4 position;
in vec3 normal;
in vec2 uv;

out vec3 vNormal;
out vec3 vPos;

void main(void)
{
	gl_Position = projection * modelView * position;
	
	// Transform vertex normal into eye coordinates (assumes modelView matrix uses uniform scale)
    vNormal = normalize((modelView * vec4(normal, 0.0)).xyz);
	vPos = (modelView * position).xyz;
}