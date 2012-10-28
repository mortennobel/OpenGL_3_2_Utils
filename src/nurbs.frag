#version 150

uniform vec4 lightPosition;
uniform vec4 color;

in vec3 vNormal;
in vec3 vPos;

out vec4 fragColor;

void main(void) {
	vec3 normal = normalize(vNormal);
	
	// compute diffuse (point)light 
	vec3 L = normalize(lightPosition.xyz - vPos);
	float Kd = max(dot(L, normal), 0.0);
	vec4  diffuse = Kd * color;
	if (gl_FrontFacing){
		fragColor = diffuse;
	} else {
		fragColor = vec4(0.0,0.0,0.0,1.0);
	}
  
}
