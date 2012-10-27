#version 150

uniform vec4 lightPosition;
uniform vec4 color;

in vec3 vNormal;
in vec3 vPos;

out vec4 fragColor;

void main(void) {
	vec3 normal;
	if (gl_FrontFacing){
		normal = vNormal;
	} else {
		normal = vNormal * -1.0;
	} 
	
	// compute diffuse (point)light 
	vec3 L = normalize(lightPosition.xyz - vPos);
	float Kd = max(dot(L, normalize(normal)), 0.0);
	vec4  diffuse = Kd * color;
  
	fragColor = diffuse;
  
}
